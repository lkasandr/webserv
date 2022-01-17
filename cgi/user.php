<head>
<meta charset="utf-8" />
</head>
<?php
$name = "не определено";
$age = "не определен";
if(isset($_REQUEST['name'])){
  
    $name = $_REQUEST['name'];
}
if(isset($_REQUEST['age'])){
  
    $age = $_REQUEST['age'];
}
// echo "Проверяем PHP <br>";
// session_start();
 
if (isset($_SESSION["name"]) && isset($_SESSION["age"]))
{
    $name = $_SESSION["name"];
    $age = $_SESSION["age"];
    echo "Name: $name <br> Age: $age";
}

?>
