const char *main_html = R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
  <title>Smart Aid Home</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    .text-center {
      text-align: center;
      background-color: grey;
    }
    .center_div {
        margin: 0 auto;
        width:15% 
    }
    #ssid-dropbox {
        width: 200px;
    }
    #password-box {
        width: 200px;
    }
    .button {
        width: 200px;
    }
  </style>
</head>
<body>

<div class="jumbotron text-center">
  <h2>Setup your smart plug</h2>
  <p>Please select the right wifi, and put in the right password</p> 
</div><br>
  
<div class="container center_div">
 <form>
 <button type="button" name="get-wifi-button">Get nearby wifi connections</button><br><br>
  Wifi SSID:<br>
  <select name="ssid" id="ssid-dropbox">
  <option value="">Click on the button above</option>
    </select>
  </select><br><br>
  Password:<br>
  <input type="password" name="password" id="password-box">
</form>
</div>
</body>
</html>

<script type="text/javascript">
$("button").click(function(){
  $.ajax({
    type: "GET",
    url: "get_ssids.php",
    success: function(result) {
        console.log(result)
        $("#ssid-dropbox").html(result);
    }
  })
})
</script>
  
)=====";
