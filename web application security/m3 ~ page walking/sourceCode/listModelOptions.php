<?php
function list_model_options($directory)
{
    // Open the directory and read its contents
    $dir_handle = opendir($directory);
    while (false !== ($file = readdir($dir_handle))) {
        // Skip current and parent directories
        if ($file == "." || $file == "..") {
            continue;
        }
        // Print the file name
        $file_no_ext = str_replace(".obj", "", $file);
        echo "<option value='$file_no_ext'>" . $file_no_ext . "</option>";
    
    }
    // Close the directory handle
    closedir($dir_handle);
}

$dir = $_GET['dir'];
if(isset($dir) && !is_null($dir)){
    list_model_options($dir);
}

?>

