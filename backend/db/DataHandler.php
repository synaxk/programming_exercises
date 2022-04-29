<?php

class DataHandler {
    private $sqlConnection;
    private $table;

    public function __construct($table) {
        /**configure sql connection*/
        $this->sqlConnection = new mysqli("localhost", "svc_appointmentfinder", "MyPassword!", "appointmentfinder");
        $this->table = $table; //table for current sql operation
    }

    /**switch tableName for sql operation*/
    public function changeTable($table) {
        $this->table = $table;
    }

    /**Generic select statement with optional whereClause*/
    public function selectAll($whereClause = "") {
        $query = "SELECT * FROM $this->table";
        if ($whereClause !== "") {
            $query .= " WHERE " . $whereClause;
        }
        /**execute query*/
        $result = $this->sqlConnection->query($query);
        $data = [];
        /**fetch assoc -> set column names as array keys*/
        /**append query results to data array*/
        while ($row = $result->fetch_assoc()) {
            $data[] = $row;
        }
        return $data;
    }

    /**Generic count with optional where claus*/
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
            $columns .= $key . ","; //example colums= "title,location,dueDate,
            $valuePlaceHolder .= "?,";
            if (preg_match('/.*ID/', $key)) { //match ID columns -> for number placeholder
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

    /**delete entry from database*/
    public function delete($whereClause) {
        $query = "DELETE FROM $this->table WHERE $whereClause";
        return $this->sqlConnection->query($query);
    }

}