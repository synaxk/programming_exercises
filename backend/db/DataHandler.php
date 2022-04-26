<?php

class DataHandler {
    private $sqlConnection;
    private $table;

    public function __construct($table) {
        $this->sqlConnection = new mysqli("localhost", "svc_appointmentfinder","MyPassword!","appointmentfinder");
        $this->table = $table;
    }

    public function changeTable($table) {
        $this->table = $table;
    }

    public function selectAll($whereClause = "") {
        $query = "SELECT * FROM $this->table";
        if ($whereClause !== "") {
            $query .= " WHERE " . $whereClause;
        }
        $result = $this->sqlConnection->query($query);
        $data = [];
        while ($row = $result->fetch_assoc()) {
            $data[] = $row;
        }
        if (count($data) == 1) {
            return $data[0];
        }
        return $data;
    }

    public function insert($data) {
        $columns = "";
        $values = "";
        $valuePlaceHolder = "";
        $paramPlaceHolder = "";
        $i = 0;
        foreach ($data as $key => $value) {
            $i++;
            $columns .= $key . ",";
            $valuePlaceHolder .= "?,";
            if (preg_match($key,'.*ID')) {
                $paramPlaceHolder .= 'i';
            } else {
                $paramPlaceHolder .= 's';
            }
            $values .= "$value,";
        }

        $columns = rtrim($columns, ',');
        $valuePlaceHolder = rtrim($valuePlaceHolder, ',');
        $values = rtrim($values, ',');

        $query = "INSERT INTO $this->table ($columns) VALUES ($valuePlaceHolder) ";
        $query = $this->sqlConnection->prepare($query);
        $query->bind_param("$paramPlaceHolder", $values);
        $query->execute();
    }

    public function update($whereClause, $key, $value) {
        $query = "UPDATE $this->table SET $key='$value' WHERE $whereClause";
        $query = $this->sqlConnection->prepare($query);
        $query->execute();
    }

    public function delete($whereClause) {
        $query = "DELETE $this->table WHERE $whereClause";
    }

}