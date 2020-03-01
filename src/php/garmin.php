<?php
require("json.php");
require("sql.php");
$mysqli = getSqlInterface();

$hibiscusSouthHeartbeat;
$hibiscusSouthHumidity;
$hibiscusSouthVoltage;
$bathroomWindowHeartbeat;
$bathroomWindowClosed;
$bathroomWindowVoltage;

$result = sqlCommand("SELECT * FROM `OrangeCedarSensors` WHERE `entryNo` IN (SELECT MAX(`entryNo`) FROM `OrangeCedarSensors` GROUP BY `mac`, `sensor`)");
while ($sensor = $result->fetch_assoc()) {
	if ($sensor["mac"] == "3C:71:BF:2A:AE:E4") {
		$hibiscusSouthHeartbeat = date("H:i", $sensor["dateTime"]);
		if ($sensor["sensor"] == 'humidity') {
			$hibiscusSouthHumidity = $sensor["value"];
		}
		if ($sensor["sensor"] == 'voltage') {
			$hibiscusSouthVoltage = $sensor["value"];
		}
	}
	if ($sensor["mac"] == "EC:FA:BC:61:F9:24") {
		$bathroomWindowHeartbeat = date("H:i", $sensor["dateTime"]);
		if ($sensor["sensor"] == 'window') {
			if ($sensor["value"] == '1') {
				$bathroomWindowClosed = "zu";
			} else {
				$bathroomWindowClosed = "auf";
			}
		}
		if ($sensor["sensor"] == 'voltage') {
			$bathroomWindowVoltage = $sensor["value"];
		}
	}
}
$result->close();
closeSqlInterface();

$output = array();
$output[0]["name"] = sprintf("Hibiskus Lang %sh", $hibiscusSouthHumidity);
$output[0]["value"] = sprintf("%s (%sv)", $hibiscusSouthHeartbeat, $hibiscusSouthVoltage);
$output[1]["name"] = sprintf("Bad Fenster %s", $bathroomWindowClosed);
$output[1]["value"] = sprintf("%s (%sv)", $bathroomWindowHeartbeat, $bathroomWindowVoltage);

echoJson($output);
?>