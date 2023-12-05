<?php
//connects to the database code
  include_once('database.php');
  $api_key_value = "tPmAT5Ab3j7F9";

//makes sure if there is a post request
  if ($_SERVER["REQUEST_METHOD"] == "POST") {
      //sanitize the api key received
    $api_key = test_input($_POST["api_key"] ?? '');
    
    //checks to make sure api key matches 
    if($api_key == $api_key_value) {
        //if so then inputs values from POST
      $sensor = test_input($_POST["sensor"] ?? ''); //gives identity to sensor
      $value1 = test_input($_POST["value1"] ?? ''); //x
      $value2 = test_input($_POST["value2"] ?? ''); //y
      $value3 = test_input($_POST["value3"] ?? ''); //z
      $bus_voltage = test_input($_POST["value4"] ?? ''); // bus_voltage
      $current_mA = test_input($_POST["value5"] ?? ''); // current_mA

      // attempts to insert the reading into the database
      try {
        $result = insertReading($sensor, $value1, $value2, $value3, $bus_voltage, $current_mA);
        echo $result;
        
      } catch (Exception $e) {
        // notes exceptions in error log within host
        error_log($e->getMessage());
        echo "Error: " . $e->getMessage();
      }
    }
    else {
      echo "wrong API Key";
    }
  }
  else {
    echo "no data with HTTP post";
  }

  function test_input($data) {
      //trims whitespace and slashes and converts characters to HTML understanding
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
  }
?>
