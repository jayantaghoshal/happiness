const express = require('express');
const bodyParser = require('body-parser');
const app = express();
var cors = require('cors');
const path = require('path');
var xml = require('xml');
const errorlog = require('./util/winston_logger').errorlog;
const successlog = require('./util/winston_logger').successlog;

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
    top_job_name = req.query.top_job_name;
    if (req.query.build_number) {
        if (top_job_name == "ihu_gate") {
            var build_number = req.query.build_number;
            var cursor = db.collection('records').find({ "job_name": "ihu_gate_test", "test_job_build_number": parseInt(build_number) })
                .project({ "top_test_job_build_number": 1, "result": 1, "test_type": 1, "job_name": 1, "test_job_build_number": 1, "module_name": 1, "test_dir_name": 1, "_id": 0 }).toArray(function(err, basic_test_detail) {
                    res.render('test_modules.ejs', { view: "list_tests_page", basic_test_detail: basic_test_detail, build_numbers: [build_number], top_job_name: top_job_name });
                });

        } else {
            var build_number = req.query.build_number;
            var cursor = db.collection('records').find({
                    "top_test_job_name": top_job_name,
                    "top_test_job_build_number": parseInt(build_number)
                })
                .project({ "top_test_job_build_number": 1, "result": 1, "test_type": 1, "job_name": 1, "test_job_build_number": 1, "module_name": 1, "test_dir_name": 1, "_id": 0 }).toArray(function(err, basic_test_detail) {
                    res.render('test_modules.ejs', { view: "list_tests_page", basic_test_detail: basic_test_detail, build_numbers: [build_number], top_job_name: top_job_name });

                });
        }
    } else {
        res.redirect('/');
    }
});

app.get('/detailed_view', cors(), (req, res) => {
    var records = db.collection('records');
    module_name = req.query.module_name;
    test_job_name = req.query.job_name;
    test_job_build_number = parseInt(req.query.build_number);
    test_dir_name = req.query.test_dir_name;

    var defaultFilterForThisTest = function() {
        return {
            "job_name": test_job_name,
            "module_name": module_name,
            "test_dir_name": test_dir_name
        };
    };

    var details = db.collection('records').find({
            "job_name": test_job_name,
            "test_job_build_number": parseInt(test_job_build_number),
            "module_name": module_name,
            "test_dir_name": test_dir_name
        })
        .project({
            _id: 0
        }).toArray().then(function(test_detail) {

            var test_record = test_detail[0];

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

            if (typeof test_record["testcases"] !== 'undefined' && test_record["testcases"]) {
                testcases_test_record = test_record["testcases"];
            } else {
                testcases_test_record = [];
            }

            return {
                test_detail: meta_test_record,
                testcases_test_record: testcases_test_record
            };
        });

    var trendProjection = {
        result: 1,
        runtime : 1,
        test_job_build_number: 1,
        started_at: 1
    };

    var trend = records.find(defaultFilterForThisTest())
        .project(trendProjection)
        .sort({ test_job_build_number: -1 })
        .limit(15)
        .toArray();


    var lastFail = records.find({
            "job_name": test_job_name,
            "module_name": module_name,
            "test_dir_name": test_dir_name,
            "result": false,
        })
        .project(trendProjection)
        .sort({ test_job_build_number: -1 })
        .limit(1)
        .toArray();

    var lastSuccess = records.find({
            "job_name": test_job_name,
            "module_name": module_name,
            "test_dir_name": test_dir_name,
            "result": true,
        })
        .project(trendProjection)
        .sort({ test_job_build_number: -1 })
        .limit(1)
        .toArray();

    var lastSuccessBeforeLastFail = lastFail.then(function(lastFailResult) {
        var filter = {
            "job_name": test_job_name,
            "module_name": module_name,
            "test_dir_name": test_dir_name,
            "result": true,
        }
        if (lastFailResult.length > 0) {
            filter["test_job_build_number"] = {"$lt": lastFailResult[0]["test_job_build_number"]};
        }
        return records.find(filter)
            .project(trendProjection)
            .sort({ test_job_build_number: -1 })
            .limit(1)
            .toArray();
    });

    var firstFailAfterSuccess = lastSuccessBeforeLastFail.then(function(lastSuccessBeforeLastFailResults) {
        var filter =  {
            "job_name": test_job_name,
            "module_name": module_name,
            "test_dir_name": test_dir_name,
            "result": false,
        };
        if (lastSuccessBeforeLastFailResults.length > 0) {
            filter["test_job_build_number"] = {"$gt": lastSuccessBeforeLastFailResults[0]["test_job_build_number"]};
        }

        return db.collection('records').find(filter)
            .project(trendProjection)
            .sort({ test_job_build_number: 1 })
            .limit(1)
            .toArray();
    });

    Promise.all([details, trend, lastSuccess, firstFailAfterSuccess, lastFail, lastSuccessBeforeLastFail]).then(
        function(promiseResults) {
            var detailResults = promiseResults[0];
            var trendResults = promiseResults[1];
            var lastSuccessResult = promiseResults[2];
            var firstFailAfterSuccessResult = promiseResults[3];
            var lastFailResult = promiseResults[4];
            var lastSuccessBeforeLastFailResults = promiseResults[5];

            res.render('detailed_view.ejs', {
                module_name: module_name,
                test_job_name: test_job_name,
                test_detail: detailResults.test_detail,
                logs_test_record: detailResults.logs_test_record,
                testcases_test_record: detailResults.testcases_test_record,
                build_trend: trendResults.reverse(),
                notable_events: [
                    { name: "Last success", value: lastSuccessResult && lastSuccessResult[0]},
                    { name: "Last fail",  value: lastFailResult && lastFailResult[0]},
                    { name: "First fail after last success", value:  firstFailAfterSuccessResult && firstFailAfterSuccessResult[0]},
                    { name: "Last success before last fail", value: lastSuccessBeforeLastFailResults && lastSuccessBeforeLastFailResults[0]}
                ]
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