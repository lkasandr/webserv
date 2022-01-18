<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
</head>
<body>
<?php
$name = "не определено";
$age = "не определен";


session_start();
$_SESSION["name"] = $name;
$_SESSION["age"] = $age;
// $visit_count = 1;
// $_SESSION["visit_count"] = $visit_count;
// echo "Данные сохранены в сессии";

// session_start();
 
if (isset($_SESSION["name"]) && isset($_SESSION["age"]) && isset($_SESSION["visit_count"]))
{
    $name = $_SESSION["name"];
    $age = $_SESSION["age"];
    $visit_count = $_SESSION["visit_count"] + 1;
    echo "Name: $name <br> Age: $age <br>";
    print("Количество посещений: " . $visit_count);
}





// session_start();
// $visit_count = 1;

if (isset($_SESSION["visit_count"])) {
    $visit_count = $_SESSION["visit_count"] + 1;
}

$_SESSION["visit_count"] = $visit_count;

print("Количество посещений: " . $visit_count);
?>
<h3>Форма ввода данных</h3>
<form method="POST">
    <p>Имя: <input type="text" name="name" /></p>
    <p>Возраст: <input type="number" name="age" /></p>
    <input type="submit" value="Отправить">
</form>
</body>
</html>