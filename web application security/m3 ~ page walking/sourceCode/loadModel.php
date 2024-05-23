<?php

function download_file($filename)
{
    $file_path = "./models/" . $filename;

    if (file_exists($file_path)) {
        
        header("Content-Type: application/octet-stream");
        header("Content-Disposition: attachment; filename=" . basename($file_path));
        header("Content-Length: " . filesize($file_path));
        readfile($file_path);
        exit;

    } else {
        die("ERROR: file " . htmlspecialchars($file_path, ENT_QUOTES, 'UTF-8') . " not found.");
    }
}

$filename = $_GET['model'];
download_file($filename);
?>
