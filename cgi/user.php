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
echo "Имя: $name <br> Возраст: $age";

//debug
print_r($_REQUEST);

?>
