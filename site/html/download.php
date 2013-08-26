<?php
// If user click the download link
if(isset($_GET['filename'])){
        // The directory of downloadable files
        // This directory should be unaccessible from web
        $file_dir="/reports/";
 
        // Replace the slash and backslash character with empty string
        // The slash and backslash character can be dangerous
        $file_name=str_replace("/", "", $_GET['filename']);
        $file_name=str_replace("\\", "", $file_name);
 
        // If the requested file is exist
        if(file_exists($file_dir.$file_name)){
                // Get the file size
                $file_size=filesize($file_dir.$file_name);
                // Open the file
                $fh=fopen($file_dir.$file_name, "r");
 
                // Download speed in KB/s
                $speed=12000;
 
                // Initialize the range of bytes to be transferred
                $start=0;
                $end=$file_size-1;
 
                // Check HTTP_RANGE variable
                if(isset($_SERVER['HTTP_RANGE']) &&
                        preg_match('/^bytes=(\d+)-(\d*)/', $_SERVER['HTTP_RANGE'], $arr)){
 
                        // Starting byte
                        $start=$arr[1];
                        if($arr[2]){
                                // Ending byte
                                $end=$arr[2];
                        }
                }
 
                // Check if starting and ending byte is valid
                if($start>$end || $start>=$file_size){
                        header("HTTP/1.1 416 Requested Range Not Satisfiable");
                        header("Content-Length: 0");
                }
                else{
                        // For the first time download
                        if($start==0 && $end==$file_size){
                                // Send HTTP OK header
                                header("HTTP/1.1 200 OK");
                        }
                        else{
                                // For resume download
                                // Send Partial Content header
                                header("HTTP/1.1 206 Partial Content");
                                // Send Content-Range header
                                header("Content-Range: bytes ".$start."-".$end."/".$file_size);
                        }
 
                        // Bytes left
                        $left=$end-$start+1;
 
                        // Send the other headers
                        header("Content-Type: application/octet-stream");
                        header("Accept-Ranges: bytes");
                        // Content length should be the bytes left
                        header("Content-Length: ".$left);
                        header("Content-Disposition: attachment; filename=".$file_name);
 
                        // Read file from the given starting bytes
                        fseek($fh, $start);
                        // Loop while there are bytes left
                        while($left>0){
                                // Bytes to be transferred
                                // according to the defined speed
                                $bytes=$speed*1024;
                                // Read file per size
                                echo fread($fh, $bytes);
                                // Flush the content to client
                                flush();
                                // Substract bytes left with the tranferred bytes
                                $left-=$bytes;
                                // Delay for 1 second
                                sleep(1);
                        }
                }
 
                fclose($fh);
        }
        else{
                // If the requested file is not exist
                // Display error message
                echo "File not found!";
        }
 
        exit();
}
?>
<html>
<head>
        <title>Home</title>
</head>
<body>
        <a href="index.php?filename=file.pdf">Download</a>
</body>
</html>
