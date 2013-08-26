

<?


 



$extensions=array('.pdf','.ppt','.docx');


 $extension=strrchr($_FILES['file']['name'],'.');
  if(!in_array($extension,$extensions))
 {
 echo("<br>unsupported format");
  exit;
 }
 if ($_FILES['file']['size']==0)
 {
 echo("<br>file has 0 byte size");
  exit;
  }
   


if (!is_uploaded_file($_FILES['file']['tmp_name']))
 {
echo"upload failed".$_FILES['file']['name'];
 exit;
 }
 $target="../reports";

if(!move_uploaded_file($_FILES["file"]["tmp_name"], "$target/".$_FILES["file"]["name"]) )
{
echo"<br>sorry could  not upload file to collect directory";
exit;
}
$rname=$_FILES["file"]["name"];
echo"<br>the file $rname was uploaded successfully as at  ";
echo date("G:i j-M-Y");
echo ("<br>thanks for the report<br>");


 
?> 


