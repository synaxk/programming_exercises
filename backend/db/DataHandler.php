<?php

class DataHandler {
    private $sqlConnection;
    private $table;

    public function __construct($table) {
        $this->sqlConnection = new mysqli("localhost", "svc_appointmentfinder", "MyPassword!", "appointmentfinder");
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


        return $data;
    }

    public function selectCount($whereClause = "") {
        $query = "SELECT COUNT(*) as count FROM $this->table";
        if ($whereClause !== "") {
            $query .= " WHERE " . $whereClause;
        }
        $result = $this->sqlConnection->query($query);
        $data = [];
        while ($row = $result->fetch_assoc()) {
            $data[] = $row;
        }
        return $data;
    }

    /**generic database insert*/
    public function insert($data) {
        $columns = "";
        $values = [];
        $valuePlaceHolder = "";
        $paramPlaceHolder = "";
        $i = 0;
        /**build parts of sql statement $key.. column names $value are actual values*/
        /**concat and separate columns and values with ','*/
        foreach ($data as $key => $value) {
            $columns .= $key . ",";
            $valuePlaceHolder .= "?,";
            if (preg_match('/.*ID/', $key)) {
                $paramPlaceHolder .= 'i';
            } else {
                $paramPlaceHolder .= 's';
            }
            $values[$i++] = $value;
        }
        /**remove last ','*/
        $columns = rtrim($columns, ',');
        $valuePlaceHolder = rtrim($valuePlaceHolder, ',');

        /**build query for prepare statement*/
        $query = "INSERT INTO $this->table ($columns) VALUES ($valuePlaceHolder) ";
        $query = $this->sqlConnection->prepare($query);
        /**bind values from $values array*/
        $query->bind_param("$paramPlaceHolder", ...$values);
        $query->execute();

        /**return the id of the inserted entry*/
        return $query->insert_id;
    }

    public function delete($whereClause) {
        $query = "DELETE FROM $this->table WHERE $whereClause";
        return $this->sqlConnection->query($query);
    }

}