<?php
require("json.php");
require("sql.php");
$mysqli = getSqlInterface();

$output = array();

$result = sqlCommand("SELECT *, (SELECT MIN(`value`) FROM `OrangeCedarSensors` asdf WHERE asdf.`mac` = `OrangeCedarSensors`.`mac` AND `sensor` = 'humidity') AS `min`, (SELECT MAX(`value`) FROM `OrangeCedarSensors` asdf WHERE asdf.`mac` = `OrangeCedarSensors`.`mac` AND `sensor` = 'humidity') AS `max` FROM `OrangeCedarSensors` WHERE `mac` = 'EC:FA:BC:63:1F:9E' AND `sensor` = 'humidity' ORDER BY dateTime desc LIMIT 1");
$sensor = $result->fetch_assoc();
$output["hibiscusSouth"] = "Hibiscus: "
 . round(($sensor["value"] - $sensor["min"]) / ($sensor["max"] - $sensor["min"]) * 100) . "%"
 . (time() - $sensor["dateTime"] > (60 * 60) ? "*" : "");
$output["hibiscusSouthSensor"] = $sensor;

$result = sqlCommand("SELECT *, (SELECT MIN(`value`) FROM `OrangeCedarSensors` asdf WHERE asdf.`mac` = `OrangeCedarSensors`.`mac` AND `sensor` = 'humidity') AS `min`, (SELECT MAX(`value`) FROM `OrangeCedarSensors` asdf WHERE asdf.`mac` = `OrangeCedarSensors`.`mac` AND `sensor` = 'humidity') AS `max` FROM `OrangeCedarSensors` WHERE `mac` = '3C:71:BF:2A:AE:E4' AND `sensor` = 'humidity' ORDER BY dateTime desc LIMIT 1");
$sensor = $result->fetch_assoc();
$output["calathea"] = "Calathea: "
 . round(($sensor["value"] - $sensor["min"]) / ($sensor["max"] - $sensor["min"]) * 100) . "%"
 . (time() - $sensor["dateTime"] > (60 * 60) ? "*" : "");
$output["calatheaSensor"] = $sensor;

$result = sqlCommand("SELECT * FROM `OrangeCedarSensors` WHERE `mac` = 'EC:FA:BC:61:F9:24' AND `sensor` = 'window' ORDER BY dateTime desc LIMIT 1");
$sensor = $result->fetch_assoc();
if ($sensor["value"] == '1') $output["bathroom"] = "Bad: zu";
else $output["bathroom"] = "Bad: offen";
$output["bathroom"] .= (time() - $sensor["dateTime"] > (60 * 60) ? "*" : "");
$output["bathroomSensor"] = $sensor;

// while ($sensor = $result->fetch_assoc()) {
// 	if ($sensor["mac"] == "EC:FA:BC:63:1F:9E") {
// 		$hibiscusSouthHeartbeat = date("H:i", $sensor["dateTime"]);
// 		if ($sensor["sensor"] == 'humidity') {
// 			$hibiscusSouthHumidity = $sensor["value"];

// 			$result2 = sqlCommand("SELECT MAX(`value`) AS `max`, MIN(`value`) as `min` FROM `OrangeCedarSensors` WHERE `mac` = 'EC:FA:BC:63:1F:9E' and `sensor` = 'humidity'");
// 			$humidityMinMax = $result2->fetch_assoc();
// 			$humidityMinMax["min"] / $humidityMinMax["max"];
// 		}
// 		if ($sensor["sensor"] == 'voltage') {
// 			$hibiscusSouthVoltage = $sensor["value"];
// 		}
// 	}
// 	if ($sensor["mac"] == "3C:71:BF:2A:AE:E4") {
// 		$calatheaHeartbeat = date("H:i", $sensor["dateTime"]);
// 		if ($sensor["sensor"] == 'humidity') {
// 			$calatheaHumidity = $sensor["value"];
// 		}
// 		if ($sensor["sensor"] == 'voltage') {
// 			$calatheaVoltage = $sensor["value"];
// 		}
// 	}
// 	if ($sensor["mac"] == "EC:FA:BC:61:F9:24") {
// 		$bathroomWindowHeartbeat = date("H:i", $sensor["dateTime"]);
// 		if ($sensor["sensor"] == 'window') {
// 			if ($sensor["value"] == '1') {
// 				$bathroomWindowClosed = "zu";
// 			} else {
// 				$bathroomWindowClosed = "auf";
// 			}
// 		}
// 		if ($sensor["sensor"] == 'voltage') {
// 			$bathroomWindowVoltage = $sensor["value"];
// 		}
// 	}
// }
// $result->close();
// closeSqlInterface();

// $output = array();
// $output[0]["name"] = sprintf("Hibiskus %sh (%s)", $hibiscusSouthHumidity, $hibiscusSouthHeartbeat);
// $output[0]["value"] = "";
// // $output[0]["value"] = sprintf("%s (%sv)", $hibiscusSouthHeartbeat, $hibiscusSouthVoltage);
// $output[1]["name"] = sprintf("Calathea %sh (%s)", $calatheaHumidity, $calatheaHeartbeat);
// $output[1]["value"] = "";
// // $output[1]["value"] = sprintf("%s (%sv)", $hibiscusSouthHeartbeat, $hibiscusSouthVoltage);
// $output[2]["name"] = sprintf("Bad %s (%s)", $bathroomWindowClosed, $bathroomWindowHeartbeat);
// $output[2]["value"] = "";
// // $output[2]["value"] = sprintf("%s (%sv)", $bathroomWindowHeartbeat, $bathroomWindowVoltage);

echoJson($output);
?>