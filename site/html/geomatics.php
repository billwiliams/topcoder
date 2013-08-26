<?
include"constants.php";



$con=$connection;
/* show tables */
$sql = "select * from engineering where course='geomatics eng'";
$query = mysqli_query($con,$sql );
 
echo "<table>";
 
//now read and display the entire row of a table one by one looping through it.
//to loop we are using While condition here
 
while( $row = mysqli_fetch_assoc($query) )
{
echo "<td>$row[university]</td>";
echo "<td>$row[course]</td>";
echo "<td>$row[file]</td>";
echo "<td>$row[date]</td>";


echo "<td>$row[id]</td>";
echo"<tr>";
}
 
echo "</table>";
 
?>

