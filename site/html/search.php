<?
require_once("../includes/constants.php");
$searchterm=trim($GET['keyname']);
if($searchterm="");
{
echo("please input a file name ");
exit;
}
$host=SERVER;
$pwd=PASSWORD;
$db=DATABASE;
$usr=USERNAME;

$link=mysqli_connect($host,$usr,$pwd,$db);
$query="SELECT *FROM WHERE LIKE '%$searchterm%'";
$results=mysqli_connect($link,$query);

?>
