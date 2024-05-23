<!DOCTYPE html>
<html lang="en">

<head>
  <!-- 
    Part of the code of this challenge comes from Prof. Gribaudo's Computer Graphics Course :-)
  -->
  <meta charset="UTF-8">
  <title>My New Renderer</title>
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css">
</head>

<body class="bg-light" onload="main();">
  <div class="container-fluid">
    <header class="p-3">
      <h1 class="text-center">My New Renderer</h1>
    </header>
    <div class="row">
      <div class="col-8">
        <canvas id="my-canvas" style="background-color: white; width: 100%; height: 100%;"></canvas>
      </div>
      <div class="col-4">
        <div class="form-group">
          <label for="options">Models</label>
          <select class="form-control" id="modelList">
            <?php
            require_once('listModelOptions.php');
            list_model_options('./models');
            ?>
          </select>
        </div>
        <button type="button" class="btn btn-primary" id="loadBtn">Render</button>
      </div>
    </div>
    <div class="row">
      <table class="table table-bordered mt-4">
        <tr>
          <td>Roll Left: <B>Q</B></td>
          <td>Turn Up: <B>Up</B> Arrow</td>
          <td>Roll Right: <B>E</B></td>
        </tr>
        <tr>
          <td>Turn Left: <B>Left</B> Arrow</td>
          <td>Turn Down: <B>Down</B> Arrow</td>
          <td>Turn Right: <B>Right</B> Arrow</td>
        </tr>
      </table>
    </div>
  </div>
  <script src="js/utils.js"></script>
  <script src="js/shaders.js"></script>
  <script src="js/webgl-obj-loader.min.js"></script>
  <script src="js/quaternion.min.js"></script>
  <script src="js/app.js"></script>
  <script src="https://code.jquery.com/jquery-3.7.0.slim.min.js"
    integrity="sha256-tG5mcZUtJsZvyKAxYLVXrmjKBVLd6VpVccqz/r4ypFE=" crossorigin="anonymous"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js"></script>
</body>

</html>
