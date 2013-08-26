<head>

<script type="text/javascript" src="jquery.js"></script>
</head> 
<link rel="stylesheet" type="text\css" href="eng.css">
<body class="bac">

<div class="menu">
<?
include'menu.php';

?>
</div>
<div class="elec" >

<br><a  href="/electrical.php" class="elc">Electrical engineering </a><br>
<br><br><a href="/mechanical.php"class="elc">Mechanical engineering</a><br>
<br><br><a href="/nanotech.php"class="elc">Nano engineering</a><br>
<br><br><a href="/computer.php"class="elc">Computer engineering</a> <br>
<br><br><a href="/civil.php"class="elc">Civil engineering</a> <br>
<br><br><a href="/geomatics.php"class="elc">Geomatics engineering</a><br>
</div>


<script type="text/javascript"><!--
$(document).ready(function() {
  
  $('a.elc').click(function() {
    var url = $(this).attr('href');
    $('#content').load(url);
    return false;
  });
});
--></script>
<div id="content">

</div>

</body>
</html>
