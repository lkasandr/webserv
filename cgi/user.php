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
echo "Проверяем PHP <br>";
echo "Имя: $name <br> Возраст: $age <br>";

//debug
// print_r($_REQUEST);

?>
