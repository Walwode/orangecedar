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
	if ($sensor["mac"] == "BC:DD:C2:FD:D9:8D") {
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
$output[] = sprintf("Hibiskus Lang %sh (%sv) %s", $hibiscusSouthHumidity, $hibiscusSouthVoltage, $hibiscusSouthHeartbeat);
$output[] = sprintf("Bad Fenster %s (%sv) %s", $bathroomWindowClosed, $bathroomWindowVoltage, $bathroomWindowHeartbeat);

echoJson($output);
?>