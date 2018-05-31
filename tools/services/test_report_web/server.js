const express = require('express');
const bodyParser = require('body-parser');
const app = express();
var cors = require('cors');
const path = require('path');
var xml = require('xml');
const errorlog = require('./util/winston_logger').errorlog;
const successlog = require('./util/winston_logger').successlog;
var redis = require('redis');
var redis_client = redis.createClient(6379, process.env.SWF1_REDIS_PORT_6379_TCP_ADDR); //creates a new redis client

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));
app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, 'views'));

process.on('uncaughtException', function(err) {
    errorlog.error(err);
});

app.use(function(err, req, res, next) {
    errorlog.error(err);
    res.status(500).send('Something broke!');
});

function compare(a, b) {
    return a - b;
}

const MONGO_SORT_ASCENDING = 1;
const MONGO_SORT_DESCENDING = -1;

var db;
const MongoClient = require('mongodb').MongoClient;
const ObjectId = require('mongodb').ObjectId;
var mongo_ip = process.env.ICUP_ANDROID_MONGO_PORT_27017_TCP_ADDR
MongoClient.connect('mongodb://' + process.env.MONGODB_USER + ':' + process.env.MONGODB_PASSWORD + '@' + mongo_ip + ':27017', (err, client) => {
    if (err) return errorlog.error(err);
    db = client.db('test_results');
    app.listen(3001, () => {
        successlog.info('listening on 3001');
    });
});

app.get('/', cors(), (req, res) => {
    res.render('home_frame.ejs');
});

app.get('/latest', cors(), (req, res) => {
    if (req.query.top_job_name == "ihu_gate") {
        build_number_name = "test_job_build_number"
        project_dict = { "test_job_build_number": 1 }
        query_dict = { "job_name": "ihu_gate_test" }
    } else {
        build_number_name = "top_test_job_build_number"
        project_dict = { "top_test_job_build_number": 1 }
        query_dict = { "top_test_job_name": req.query.top_job_name }
    }

    var details = db.collection('records').find(query_dict).project(project_dict)
        .sort({ test_job_build_number: -1 })
        .limit(1)
        .toArray().then(function(test_detail) {
            build_number = test_detail[0][build_number_name]
            res.redirect('/tests?top_job_name=' + req.query.top_job_name + "&build_number=" + build_number);
        });
});


app.get('/home', cors(), (req, res) => {
    var cursor = db.collection('records').distinct("top_test_job_name")
        .then(function(top_test_job_names) {
            top_test_job_names.push("ihu_gate");
            res.render('index.ejs', { view: "top_job_page", top_job_names: top_test_job_names });
        });
});

app.get('/jobs', cors(), (req, res) => {
    top_job_name = req.query.top_job;
    if (top_job_name == "ihu_gate") {
        var cursor = db.collection('records').find({ job_name: "ihu_gate_test" }).project({ "test_job_build_number": 1, "_id": 0 }).toArray(function(err, build_numbers) {
            var extracted_build_number = [];
            for (var list_index in build_numbers) {
                for (var key in build_numbers[list_index]) {
                    extracted_build_number.push(build_numbers[list_index][key]);
                }
            }

            var unique_build_numbers = new Set(extracted_build_number);

            var array_unique_build_numbers = Array.from(unique_build_numbers);
            array_unique_build_numbers.sort(compare).reverse();
            res.render('index.ejs', { view: "select_build_page", build_numbers: array_unique_build_numbers, top_job_name: req.query.top_job });


        });
    } else {

        var cursor = db.collection('records').find({ top_test_job_name: top_job_name }).project({ "top_test_job_build_number": 1, "_id": 0 }).toArray(function(err, build_numbers) {
            var extracted_build_number = [];
            for (var list_index in build_numbers) {
                for (var key in build_numbers[list_index]) {
                    extracted_build_number.push(build_numbers[list_index][key]);
                }
            }

            var unique_build_numbers = new Set(extracted_build_number);

            var array_unique_build_numbers = Array.from(unique_build_numbers);
            array_unique_build_numbers.sort(compare).reverse();
            res.render('index.ejs', { view: "select_build_page", build_numbers: array_unique_build_numbers, top_job_name: req.query.top_job });


        });
    }
});

app.get('/tests/', cors(), (req, res) => {
    let query = {};
    let job_name = req.query.job_name;
    let name = ""
    if (req.query.top_job_name) {
        if (req.query.top_job_name == "ihu_gate") {
            job_name = "ihu_gate_test"
            name = "ihu_gate"
            query["test_job_build_number"] = parseInt(req.query.build_number);
        } else {
            query["top_test_job_name"] = req.query.top_job_name;
            query["top_test_job_build_number"] = parseInt(req.query.build_number);
            name = req.query.top_job_name;
        }
    }

    if (job_name)
        query["job_name"] = job_name;

    let projection = {
        "top_test_job_build_number": 1,
        "result": 1,
        "test_type": 1,
        "job_name": 1,
        "test_job_build_number": 1,
        "module_name": 1,
        "test_dir_name": 1,
        "capabilities": 1,
        "runtime": 1,
        "started_at": 1,
        "finished_at": 1,
        "_id": 1
    };
    if (Object.keys(query).length === 0) {
        res.redirect('/');
        return;
    }

    db.collection('records').find(query)
        .project(projection)
        .toArray(function(err, basic_test_detail) {
            res.render('test_modules.ejs', {
                view: "list_tests_page",
                basic_test_detail: basic_test_detail,
                build_numbers: [req.query.build_number],
                top_job_name: name,
            });
        });
});

app.get('/detailed_view', cors(), (req, res) => {
    var records = db.collection('records');
    let detailQuery = records.findOne({ "_id": ObjectId(req.query.id) })


    let allTrends = detailQuery.then(function(record) {
        let trendQuery = function(additional_queries) {
            let query = {
                "job_name": record["job_name"],
                "module_name": record["module_name"],
                "test_dir_name": record["test_dir_name"]
            };
            Object.keys(additional_queries).forEach((k) => {
                query[k] = additional_queries[k];
            });
            return records.find(query).project({
                result: 1,
                runtime: 1,
                test_job_build_number: 1,
                started_at: 1,
                _id: 1
            });
        };

        var trend = trendQuery({})
            .sort({ test_job_build_number: -1 })
            .limit(15)
            .toArray();

        var lastFail = trendQuery({ "result": false })
            .sort({ test_job_build_number: -1 })
            .limit(1)
            .toArray();

        var lastSuccess = trendQuery({ "result": true })
            .sort({ test_job_build_number: -1 })
            .limit(1)
            .toArray();

        var lastSuccessBeforeLastFail = lastFail.then(function(lastFailResult) {
            var filter = {
                "result": true,
            }
            if (lastFailResult.length > 0) {
                filter["test_job_build_number"] = { "$lt": lastFailResult[0]["test_job_build_number"] };
            }
            return trendQuery(filter)
                .sort({ test_job_build_number: -1 })
                .limit(1)
                .toArray();
        });

        var firstFailAfterSuccess = lastSuccessBeforeLastFail.then(function(lastSuccessBeforeLastFailResults) {
            var filter = {
                "result": false,
            };
            if (lastSuccessBeforeLastFailResults.length > 0) {
                filter["test_job_build_number"] = { "$gt": lastSuccessBeforeLastFailResults[0]["test_job_build_number"] };
            }
            return trendQuery(filter)
                .sort({ test_job_build_number: 1 })
                .limit(1)
                .toArray();
        });
        return Promise.all([trend, lastSuccess, firstFailAfterSuccess, lastFail, lastSuccessBeforeLastFail]);
    });

    var all_job_names_query = records.distinct("job_name");

    // Find the results of the same test case when run in another job
    var sibling_tests_query = Promise.all([detailQuery, all_job_names_query]).then(function(promiseResults) {
        let record = promiseResults[0];
        let all_job_names = promiseResults[1];
        return Promise.all(all_job_names.map(job_name =>
            records.find({
                "job_name": job_name,
                "module_name": record["module_name"],
                "test_dir_name": record["test_dir_name"]
            })
            .project({
                result: 1,
                runtime: 1,
                test_job_build_number: 1,
                started_at: 1,
                _id: 1
            })
            .sort({ test_job_build_number: MONGO_SORT_DESCENDING })
            .limit(5)
            .toArray()
            .then(result => ({ "name": job_name, "results": result.reverse() }))
        ));
    });


    Promise.all([detailQuery, allTrends, sibling_tests_query]).then(
        function(promiseResults) {
            var test_record = promiseResults[0];
            let allTrendResults = promiseResults[1];
            let sibling_tests_results = promiseResults[2];

            var trendResults = allTrendResults[0];
            var lastSuccessResult = allTrendResults[1];
            var firstFailAfterSuccessResult = allTrendResults[2];
            var lastFailResult = allTrendResults[3];
            var lastSuccessBeforeLastFailResults = allTrendResults[4];


            var meta_test_record = {};
            meta_test_record["Module"] = test_record["module_name"];
            meta_test_record["Test Directory"] = test_record["test_dir_name"];
            meta_test_record["Test Type"] = test_record["test_type"];
            meta_test_record["Top Job"] = test_record["top_test_job_name"];
            meta_test_record["top_test_job_build_number"] = test_record["top_test_job_build_number"];
            meta_test_record["Test Job"] = test_record["job_name"];
            meta_test_record["test_job_build_number"] = test_record["test_job_build_number"];
            meta_test_record["Result"] = test_record["result"];
            meta_test_record["Capabilities"] = test_record["capabilities"];
            meta_test_record["Host"] = test_record["hostname"];
            meta_test_record["Runtime"] = test_record["runtime"];
            meta_test_record["started_at"] = test_record["started_at"];
            meta_test_record["KPI"] = test_record["kpis"];
            meta_test_record["screenshots"] = test_record["screenshots"];
            meta_test_record["description"] = test_record["description"];
            meta_test_record["logs"] = test_record["logs"];
            meta_test_record["files"] = test_record["files"];

            if (typeof test_record["testcases"] !== 'undefined' && test_record["testcases"]) {
                testcases_test_record = test_record["testcases"];
            } else {
                testcases_test_record = [];
            }

            res.render('detailed_view.ejs', {
                module_name: test_record["module_name"],
                test_job_name: test_record["top_test_job_name"],
                test_detail: meta_test_record,
                testcases_test_record: testcases_test_record,
                build_trend: trendResults.reverse(),
                notable_events: [
                    { name: "Last success", value: lastSuccessResult && lastSuccessResult[0] },
                    { name: "Last fail", value: lastFailResult && lastFailResult[0] },
                    { name: "First fail after last success", value: firstFailAfterSuccessResult && firstFailAfterSuccessResult[0] },
                    { name: "Last success before last fail", value: lastSuccessBeforeLastFailResults && lastSuccessBeforeLastFailResults[0] }
                ],
                sibling_trends: sibling_tests_results
            });
        }
    );
});

app.get('/log', cors(), (req, res) => {
    db.collection('logs').findOne({ "_id": ObjectId(req.query.id) }, { "contents": 1 })
        .then(function(log) {
            res.set('Content-Type', 'text/plain');
            res.send(new Buffer(log["contents"]));
        });
});


app.get('/screenshot', cors(), (req, res) => {
    db.collection('screenshots').findOne({ "_id": ObjectId(req.query.id) }, { "data": 1 })
        .then(function(screenshot) {
            res.set('Content-Type', 'image/png');
            res.send(screenshot["data"].buffer);
        });
});

app.get('/partnumber', cors(), (req, res) => {
    res.render('partnumber_lookup.ejs');
});

app.get('/lookup_partnumber', cors(), (req, res) => {

    build_name = req.query.build_name;
    build_number = req.query.build_number;
    if (build_name == "Hourly") {
        job_name = "ihu_hourly_build-eng";
    } else if (build_name == "Daily") {
        job_name = "ihu_daily_build_vcc_eng";
    }
    redis_client.get("icup_android.jenkins." + job_name + "." + build_number + ".mp_part_number", function(err, reply) {
        res.setHeader('Content-Type', 'application/json');
        res.send(JSON.stringify({ MP_PARTNUMBER: reply }, null, 3));
    });

});

app.get('/table_partnumber', cors(), (req, res) => {
    key_value_pair = { "data": [] };
    key_sets = [];

    redis_client.send_command("keys", ["icup_android.jenkins.*.*.mp_part_number"], function(err, reply) {
        reply.sort();
        key_sets = reply;

        function get_values_from_key(keys) {
            redis_client.mget(keys, function(error, reply_values) {
                for (var i = 0; i < keys.length; i++) {
                    var job_name = keys[i].split(".")[2]; // example: ihu_hourly_build
                    var dir_name = keys[i].split(".")[3] + "_" + reply_values[i]; // example: buildnumber_partnumber(190_33423423AA)
                    key_value_pair["data"].push([keys[i], reply_values[i], "<a href=https://swf1.artifactory.cm.volvocars.biz/artifactory/ICUP_ANDROID_CI/" + job_name + "/" + dir_name + " target=_blank>VBF Files</a>"])
                }
                res.setHeader('Content-Type', 'application/json');
                res.send(JSON.stringify(key_value_pair, null, 3));
            });

        }

        get_values_from_key(key_sets);
    });




});