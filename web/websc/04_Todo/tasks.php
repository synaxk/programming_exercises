<?php
$response = getTaskList();
echo $response;

function getTaskList() {
    $data = ["Task 1", "Task 2", "Task 3"];
    return json_encode($data);
}