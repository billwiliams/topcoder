
<html>



<title>collegereports</title>
 <head>
 <script type="text/javascript" src="jquery.js"></script>
 <link rel="stylesheet" type="text\css" href="style.css">
 </head>
   
    <body>
    <script type="text/javascript"><!--
$(document).ready(function() {
  
  $('upload').submit(function() {
    var url = 'upload.php';
    $('#div1').load(url);
    return false;
  });
});
--></script>
        <header class="site-header">
          <a class="logo" href="http://www.google.com">College Reports</a>
         </header>
         <div class="menu">
      <?php include 'menu.php'; ?>
       </div>
       <link rel="stylesheet" type="text\css" href="search.css">
       
       <header class="search">
       <form action="search.php">
  Search reports: <input type="search" name="report search"> <input type="submit">
  </header>
  
</form><br>
    
        
      <p class="word">reports</p>
    
     <div class="upload">
     
     <ul>
      <li>if you would like to submit a report please upload it </a></li>
      

<form id ="create" action="upload.php" method="post"
enctype="multipart/form-data">
<label for="file">Report:</label>


<input type="file" name="file" id="file"><br>
<label for="submit">Submit:</label>
<input id="load" class="input" type="submit" name="submit" value="Submit">
<script>

</script>
<div id="div1"> </div>

</form>


 
      <li>the report should be either of the categories listed</li>
      <li>remember to leave an email so that we can gt back to you</li>
      <li>thanks alot visit <a href=" http://www.billwilliams.com">billwilliams.com for more</a></li>
      </ul>
     </div>
             
     
     
    </body>
 
</html>
