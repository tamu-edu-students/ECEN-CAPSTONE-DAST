<?php
$servername = "localhost";
$dbname = "database";
$username = "espmcu";
$password = "Tamu2023!";

// establishes connection to database
function getDBConnection() {
    global $servername, $dbname, $username, $password;
    try {
        //creating a new pdo connection
        $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
        // setting error mode to exception
        $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        return $conn;
    } catch(PDOException $e) {
        //output error if there is no connection being made
        die("connection failed: " . $e->getMessage());
    }
}

// inserts sensor data into database table
function insertReading($sensor, $value1, $value2, $value3, $bus_voltage, $current_mA) {
    try {
        //get db connection
        $conn = getDBConnection();

        //executes an insert statement with the parameters mentioned below: 
        $stmt = $conn->prepare("INSERT INTO SensorData (sensor, value1, value2, value3, bus_voltage, current_mA) VALUES (?, ?, ?, ?, ?, ?, ?)");
        $stmt->execute([$sensor, $value1, $value2, $value3, $bus_voltage, $current_mA]);
        echo "new record created";

    //output error
    } catch(PDOException $e) {
        echo "error: " . $e->getMessage();
    }
}

// gets # of most recent sensor reading from database
function getAllReadings($limit) {
    try {
        $conn = getDBConnection(); //tries to reach db connection
        //returns rows fetched into an array
        $stmt = $conn->prepare("SELECT id, sensor, value1, value2, value3, bus_voltage, current_mA, reading_time FROM SensorData ORDER BY reading_time DESC LIMIT ?");
        $stmt->execute([$limit]);
        return $stmt->fetchAll(PDO::FETCH_ASSOC);

    //returns false if an error occurs    
    } catch(PDOException $e) {
        return false;
    }
}

// Function to get the last reading to input into website using arrays
function getLastReadings() {
    return getAllReadings(1)[0] ?? false;
}

?>
