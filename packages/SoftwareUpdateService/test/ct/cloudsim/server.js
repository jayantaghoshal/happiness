var jsonServer = require('/usr/lib/node_modules/json-server');
var js2xmlparser = require('/usr/lib/node_modules/js2xmlparser');
var server = jsonServer.create();
var router = jsonServer.router(__dirname + '/db.json');
var middlewares = jsonServer.defaults({ static: __dirname + '/public' });

var db = router.db;

var routes = require(__dirname + '/routes.json');
server.use(jsonServer.rewriter(routes));

server.use(middlewares);
server.use(jsonServer.bodyParser);

router.render = function (req, res) {
  var data = res.locals.data;
  var tag = "";
  if (req._parsedUrl.pathname == '/entrypoint') {
    tag = "entry_point";
  } else if (req._parsedUrl.pathname == '/features-1') {
    tag = "features";
  } else if (req._parsedUrl.pathname == '/softwaremanagement-1') {
    tag = "software_management";
  } else if (req._parsedUrl.pathname == '/softwaremanagement-1/') {
    tag = "software_management";
  } else if (req._parsedUrl.pathname == '/availableUpdates') {
    tag = "available_updates";
  } else if (req._parsedUrl.pathname == '/availableAccessories') {
    tag = "available_accessories";
  } else if (req._parsedUrl.pathname == '/commission') {
    tag = "commission";
  } else if (req._parsedUrl.pathname == '/downloads') {
    tag = "downloads";
  }
  else {
    return res.send(data);
  }
  var str = js2xmlparser.parse(tag, data);
  res.send(str);
}

/**
 * Find matching object
 * @param {what to match on} matcher
 * @param {tag to check match on} tag
 * @param {list to find object in} list
 */
function findMatchingObj(matcher, tag, list) {
  for (i = 0; i < list.value().length; i++) {
    if (list.value()[i][tag] == matcher) {
      return list.value()[i];;
    }
  }
}

/**
 * Create installation order by prefix
 * @param {counter to prefix new installation order to ensure unique ids} counter
 */
function createInstallationOrderId(counter) {
  var counter_uuid_format_prefix = counter.toString(16);
  while (counter_uuid_format_prefix.length < 8) {
    counter_uuid_format_prefix = '0' + counter_uuid_format_prefix;
  }
  var id_suffix = "-caf6-4710-05a9-472200ad680c";
  var counter_uuid_format = counter_uuid_format_prefix + id_suffix;
  return counter_uuid_format;
}

/**
 * Add a new insstallation order to obj
 * @param {object to add installation order to} obj
 * @param {id to use in the installation order} id
 */
function addInstallationOrder(obj, id) {
  var installation_order_data = {};
  installation_order_data['id'] = id;
  installation_order_data['downloads_uri'] = "/downloads";
  installation_order_data['install_notifications_uri'] = "/installnotifications";
  installation_order_data['installation_report_uri'] = "/installationreport";
  installation_order_data['downloads_uri'] = "/downloads/" + id;

  obj['installation_order'] = installation_order_data
  obj['status'] = "COMMISSIONED";
  return obj;
}

function addActionRequest(obj, type, reason) {
  var action_request = {};
  action_request['type'] = type;
  action_request['client_id'] = "1FTKR1EDXBPB10452";
  action_request['reason'] = reason;
  action_request['created'] = "2018-05-24T10:20:00";

  obj['action_request'] = action_request;
  return obj;
}

/**
 * Get value of tag in xml
 * @param {xml} xml
 * @param {tag} tag
 */
function getValue(xml, tag) {
  var startTag = '<' + tag + '>';
  var endTag = '</' + tag + '>';
  startPos = xml.indexOf(startTag) + startTag.length;
  endPos = xml.indexOf(endTag);
  return xml.substring(startPos, endPos);
}

/**
 * Count occurence of specified substring in string
 * @param {string to find nr of occurences in} main_str
 * @param {string to count occurence of} sub_str
 */
function count(main_str, sub_str) {
  main_str += '';
  sub_str += '';

  if (sub_str.length <= 0) {
    return main_str.length + 1;
  }

  subStr = sub_str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
  return (main_str.match(new RegExp(subStr, 'gi')) || []).length;
}

server.post('/availableUpdates', function (req, res, next) {
  return res.status(405).send({ error: "Nope, cant do that." });
});


var counter = -1;
server.post('/commission', function (req, res, next) {

  var updates = db.get('availableUpdates').get('software');
  var accessories = db.get('availableAccessories').get('software');
  var found = false;

  var assignment = findMatchingObj(req.body.id, 'id', updates);
  if (assignment != null) {
    counter++;
    console.log(assignment)
    addInstallationOrder(assignment, createInstallationOrderId(counter));
    addActionRequest(assignment, "INSTALLATION", req.body.reason);
    found = true;
  }

  //If software was not found in availableUpdates, iterate over availableAccessories
  //if software found that matches id in body => add installation order to software and change status to COMMISSIONED
  if (!found) {
    var assignment = findMatchingObj(req.body.id, 'id', accessories);
    if (assignment != null) {
      counter++;
      addInstallationOrder(assignment, createInstallationOrderId(counter));
      found = true;
    }
  }

  //Find software that matches the id in body in preDownloadInfo
  if (found) {
    var preDownloadInfos = db.get('preDownloadInfo');
    var preDownloadInfoObj = findMatchingObj(req.body.id, 'id', preDownloadInfos);

    //Copy the software, add installationorder and put it in downloads
    if (preDownloadInfoObj != null) {
      //assignment['id'] = req.body.id;
      preDownloadInfoObj['installation_order_id'] = createInstallationOrderId(counter);
      db.get('downloads').push(preDownloadInfoObj).write();
    }

    //For all other assignments: set status to NON-COMMISSIONABLE
    for (i = 0; i < updates.value().length; i++) {
      if (updates.value()[i]['id'] != req.body.id) {
        assignment = updates.value()[i];
        assignment['status'] = "NON-COMMISSIONABLE";
        delete assignment.commission_uri;
      }
    }
    //For all other assignments: set status to NON-COMMISSIONABLE
    for (i = 0; i < accessories.value().length; i++) {
      if (accessories.value()[i]['id'] != req.body.id) {
        assignment = accessories.value()[i];
        assignment['status'] = "NON-COMMISSIONABLE";
        delete assignment.commission_uri;
      }
    }

    db.get('availableUpdates').get('software').write();
    db.get('availableAccessories').get('software').write();
  }

  req.method = 'GET';
  next();
});

server.get('/downloads', function (req, res, next) {
  var downloads = db.get('downloads');
  var downloadObj = findMatchingObj(req.query.id, 'installation_order_id', downloads);
  var str = js2xmlparser.parse("downloads", downloadObj);
  return res.send(str);
});

var idCounter = 1000;
server.post('/installNotification', function (req, res, next) {

  var installNotificationObj = {};

  var xml = req.body['<?xml version'];
  var tag = "id";
  installNotificationObj['software_id'] = getValue(xml, tag);

  tag = "installation_order_id";
  installNotificationObj[tag] = getValue(xml, tag);

  var notificationObj = {};
  tag = "status_code";
  notificationObj[tag] = getValue(xml, tag);

  tag = "sub_status_code";
  notificationObj[tag] = getValue(xml, tag);

  notificationObj['id'] = idCounter;
  idCounter++;
  notificationObj['created'] = "2017-05-30T09:00";
  notificationObj['timestamp'] = "2017-05-30T09:00";
  installNotificationObj['notification'] = notificationObj;

  db.get('installNotification').push(installNotificationObj).write();

  req.method = 'GET';
  next();
})

server.get('/installNotification', function (req, res, next) {
  var installNotifications = db.get('installNotification');
  var installNotificationObj = findMatchingObj(req.query.installation_order_id, 'installation_order_id', installNotifications);

  var str = js2xmlparser.parse("install_notifications", installNotificationObj);
  return res.send(str);
});



server.post('/installationReport', function (req, res, next) {
  var t = {};
  var installationReportObj = {};
  t = req.body;
  var xml = req.body['<?xml version'];

  var tag = "installation_order_id";
  installationReportObj[tag] = getValue(xml, tag);
  var installation_order_id = getValue(xml, tag);

  tag = "timestamp";
  installationReportObj[tag] = getValue(xml, tag);

  tag = "report_reason";
  installationReportObj[tag] = getValue(xml, tag);

  var downloadSummaryObj = {};
  var downloadSummary = getValue(xml, "download_summary");
  tag = "timestamp";
  downloadSummaryObj[tag] = getValue(downloadSummary, tag);

  tag = "total_download_time";
  downloadSummaryObj[tag] = getValue(downloadSummary, tag);

  tag = "effective_download_time";
  downloadSummaryObj[tag] = getValue(downloadSummary, tag);

  var i = 0;
  var subXml = getValue(downloadSummary, "data_file");
  var dataFilesArray = []
  while (i != count(downloadSummary, "<data_file>")) {
    var dataFileObj = {};
    var dataFile = subXml;
    tag = "identifier";
    dataFileObj[tag] = getValue(dataFile, tag);

    tag = "target_storage_id";
    dataFileObj[tag] = getValue(dataFile, tag);

    tag = "status";
    dataFileObj[tag] = getValue(dataFile, tag);
    dataFilesArray[i] = dataFileObj;

    ++i;
    subXml = downloadSummary.substring(downloadSummary.indexOf(subXml) + subXml.length + "</data_file>".length);
    subXml = getValue(subXml, "data_file");
  }

  downloadSummaryObj['data_file'] = dataFilesArray;
  installationReportObj["download_summary"] = downloadSummaryObj;

  var installationSummaryObj = {};
  var installationSummary = getValue(xml, "installation_summary");

  tag = "software_id";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  tag = "timestamp";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  tag = "repeat_resets";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  tag = "total_installation_time";
  installationSummaryObj[tag] = getValue(installationSummary, tag);

  i = 0;
  subXml = getValue(installationSummary, "ecu");
  var ecuArray = [];

  while (i != count(installationSummary, "<ecu>")) {
    var ecuObj = {}
    var ecu = subXml;

    tag = "ecu_address";
    ecuObj[tag] = getValue(ecu, tag);

    tag = "ecu_retries";
    ecuObj[tag] = getValue(ecu, tag);

    tag = "ecu_status";
    ecuObj[tag] = getValue(ecu, tag);

    j = 0;
    var subSubXml = getValue(ecu, "software_part");
    var softwarepartArray = [];
    while (j != count(ecu, "<software_part>")) {
      var softwarepartObj = {};
      var softwarepart = subSubXml;

      tag = "identifier";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      tag = "retries";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      tag = "measured_installation_time";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      tag = "status";
      softwarepartObj[tag] = getValue(softwarepart, tag);

      softwarepartArray[j] = softwarepartObj;
      ++j;
      subSubXml = ecu.substring(ecu.indexOf(subSubXml) + subSubXml.length + "</software_part>".length);
      subSubXml = getValue(subSubXml, "software_part");
    }
    ecuObj["software_part"] = softwarepartArray;
    ecuArray[i] = ecuObj;
    ++i;
    subXml = ecu.substring(ecu.indexOf(subXml) + subXml.length + "</ecu>".length);
    subXml = getValue(subXml, "ecu");
  }

  installationSummaryObj['ecu'] = ecuObj;
  installationReportObj["installation_summary"] = installationSummaryObj;
  db.get("installationReport").push(installationReportObj).write();

  req.method = 'GET';
  next();

  //"Close" installation order by removing from available updates and make other assignments COMMISSIONABLE
  var updates = db.get('availableUpdates').get('software');
  var assignment = findMatchingObj('COMMISSIONED', 'status', updates);

  if (assignment != null) {
    if (assignment['installation_order']['id'] == installation_order_id) {
      updates.remove(assignment).write();
    }
  }

  db.get('availableUpdates').get('software').write();
  updates = db.get('availableUpdates').get('software');

  for (i = 0; i < updates.value().length; i++) {
    var assignment = updates.value()[i];
    assignment['status'] = "COMMISSIONABLE";
    assignment['commission_uri'] = "commission";
  }

  db.get('availableUpdates').get('software').write();


  //"Close" installation order by removing from available accessories and make other assignments COMMISSIONABLE
  var accessories = db.get('availableAccessories').get('software');
  var assignment = findMatchingObj('COMMISSIONED', 'status', accessories);
  if (assignment != null) {
    if (assignment['installation_order']['id'] == installation_order_id) {
      accessories.remove(assignment).write();
    }
  }

  db.get('availableAccessories').get('software').write();
  accessories = db.get('availableAccessories').get('software');

  for (i = 0; i < accessories.value().length; i++) {
    var assignment = accessories.value()[i];
    assignment['status'] = "COMMISSIONABLE";
    assignment['commission_uri'] = "commission";
  }

  db.get('availableAccessories').get('software').write();

});

server.get('/availableUpdates', function (req, res, next) {
  var updates = db.get('availableUpdates').get('software');
  var assignment = {};
  if (req.query.id) {
    for (i = 0; i < updates.value().length; i++) {
      if (req.query.installation_order_id && updates.value()[i]['installation_order']) {
        if ((updates.value()[i]['id'] == req.query.id) && (updates.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
          assignment['software'] = updates.value()[i];
        }
      }
      else if (!req.query.installation_order_id) {
        if (updates.value()[i]['id'] == req.query.id) {
          assignment['software'] = updates.value()[i];
        }
      }
    }
  }

  else if (req.query.installation_order_id) {
    for (i = 0; i < updates.value().length; i++) {
      if ((updates.value()[i]['installation_order']) && (updates.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
        assignment['software'] = updates.value()[i];
      }
    }
  }
  else {
    assignment['software'] = updates.value()
  }

  var str = js2xmlparser.parse("available_updates", assignment);
  return res.send(str);
});

server.get('/availableAccessories', function (req, res, next) {
  var accessories = db.get('availableAccessories').get('software');
  var assignment = {};
  if (req.query.id) {
    for (i = 0; i < accessories.value().length; i++) {
      if (req.query.installation_order_id && accessories.value()[i]['installation_order']) {
        if ((accessories.value()[i]['id'] == req.query.id) && (accessories.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
          assignment['software'] = accessories.value()[i];
        }
      }
      else if (!req.query.installation_order_id) {
        if (accessories.value()[i]['id'] == req.query.id) {
          assignment['software'] = accessories.value()[i];
        }
      }
    }
  }

  else if (req.query.installation_order_id) {
    for (i = 0; i < accessories.value().length; i++) {
      if ((accessories.value()[i]['installation_order']) && (accessories.value()[i]['installation_order']['id'] == req.query.installation_order_id)) {
        assignment['software'] = accessories.value()[i];
      }
    }
  }
  else {
    assignment['software'] = accessories.value()
  }

  var str = js2xmlparser.parse("available_accessories", assignment);
  return res.send(str);
});

server.use(router);
server.listen(3000, function () {
  console.log('JSON Server is running');
});