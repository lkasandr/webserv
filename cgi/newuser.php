<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
</head>
<body>
<?php
$name = "не определено";

session_start();
$visit_count = 1;
if(isset($_POST['name']) && $_POST['name'] != "") {
    $_SESSION['name'] = $_POST['name'];
}
if (isset($_SESSION["visit_count"])) {
    $visit_count = $_SESSION["visit_count"] + 1;
}
if (isset($_SESSION["name"])) {
    $name = $_SESSION["name"];
    echo "Hello $name <br> ";
}
$_SESSION["visit_count"] = $visit_count;
$name = $_POST['name'];
echo "Name: $name <br> ";
print("Количество посещений: " . $visit_count);
?>
<h3>Форма ввода данных</h3>
<form method="POST">
    <p>Имя: <input type="text" name="name" /></p>
    <input type="submit" value="Отправить">
</form>
</body>
</html>