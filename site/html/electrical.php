<?
include 'constants.php';


$con=$connection;
/* show  rows from table egineering where column course is named electrical*/
$sql = "select * from engineering where course='electrical eng'||'electrical engineering'";
$query = mysqli_query($con,$sql );
?> 
<table border="0" cellpadding="11" cellspacing="2" >
<?

 
//now read and display the entire row of a table one by one looping through it.
//to loop we are using While condition here
//display the columns  university course labname unitname and id
echo"<p > university &nbsp;&nbsp;&nbsp course &nbsp;&nbsp  labname &nbsp;&nbsp    unitname &nbsp;&nbsp   date &nbsp;&nbsp   id</p>";
 
while( $row = mysqli_fetch_assoc($query) )
{
//display rows from the database
echo " <td>$row[university]</td> ";

echo "<td>$row[course]</td> ";

echo "<td>$row[lab]</td>";

echo "<td>$row[unit]</td>";

echo "<td>$row[date]</td>";


echo "<td>$row[id]</td>";
echo"<tr>";
echo"<tr>";
echo"<tr>";
echo"</tr>";
}
 
echo "</table>"; 
?>

