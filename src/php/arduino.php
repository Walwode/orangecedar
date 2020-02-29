<?php
require("json.php");
require("sql.php");
$mysqli = getSqlInterface();

createRequest();
switch (strtoupper($_SERVER['REQUEST_METHOD'])) {
	case 'GET': handleGet(); break;
	case 'POST': handlePost(); break;
}

function createRequest() {
	$dateTime = time();
	$content = toValidSqlValue(file_get_contents('php://input'));
	$method = $_SERVER['REQUEST_METHOD'];
	$response = $content;
	$status = 200;

	updateSqlCommand("INSERT INTO OrangeCedarRequests (dateTime, content, method, response, status) VALUES ('$dateTime', '$content', '$method', '$response', '$status')");
}

function handleGet() {
	echo "get....";
}

function handlePost() {
	$data = json_decode(file_get_contents('php://input'));
	
	$dateTime = time();
	$mac = toValidSqlValue($data->mac);

	if ($data->humidity) {
		$sensor = 'humidity';
		$value = $data->humidity;
		updateSqlCommand("INSERT INTO OrangeCedarSensors (dateTime, mac, sensor, value) VALUES ('$dateTime', '$mac', '$sensor', '$value')");
	}

	if ($data->voltage) {
		$sensor = 'voltage';
		$value = $data->voltage;
		updateSqlCommand("INSERT INTO OrangeCedarSensors (dateTime, mac, sensor, value) VALUES ('$dateTime', '$mac', '$sensor', '$value')");
	}
	
	$sensor = 'window';
	$value = $data->window;
	updateSqlCommand("INSERT INTO OrangeCedarSensors (dateTime, mac, sensor, value) VALUES ('$dateTime', '$mac', '$sensor', '$value')");
	
	echoJson($data);
}
?>