<?php
$servername = "localhost";
$username = "root";
$password = "#Include<>";
$dbname = "buspass_server_db";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$name = $_POST['name'];
$pass_no = $_POST['pass_no'];
$fprint = $_POST['fprint'];
$day = $_POST['day'];
$month = $_POST['month'];
$year = $_POST['year'];
$mobile_no = $_POST['mobile_no'];
$email_id = $_POST['email_id'];
$gender = $_POST['gender'];
$college_name = $_POST['college_name'];
$semester = $_POST['semester'];
$usn = $_POST['usn'];
$pday = $_POST['pday'];
$pmonth = $_POST['pmonth'];
$pyear = $_POST['pyear'];
$route = $_POST['route'];

$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
    if($check !== false) {
        echo "File is an image - " . $check["mime"] . ".";
        $uploadOk = 1;
    } else {
        echo "File is not an image.";
        $uploadOk = 0;
    }
}
// Check if file already exists
if (file_exists($target_file)) {
    echo "Sorry, file already exists.";
    $uploadOk = 0;
}
// Check file size
if ($_FILES["fileToUpload"]["size"] > 500000) {
    echo "Sorry, your file is too large.";
    $uploadOk = 0;
}
// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
    echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
    $uploadOk = 0;
}
// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
    } else {
        echo "Sorry, there was an error uploading your file.";
    }
}
$sql = "INSERT INTO user_data VALUES ('$name','$pass_no','$fprint','$year-$month-$day','$mobile_no','$email_id','$gender','$target_file','$college_name','$semester','$usn','$pyear-$pmonth-$pday','0','$route')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}
echo '<p><b>Please verify the following details are correct.</b></p><br><br>';
echo '<table width=50%,height=25%><style>
table,td
{
border-style:solid;
border-width:1px;
border-color:purple;
text-align:center;
}
</style><tr><th>NAME</th><th>DOB</th><th>GENDER</th><th>MOBILE</th><th>EMAIL-ID</th><th>COLLEGE</th><th>Semester</th><th>USN</th></tr>'; 
echo '<tr><td>'.$name.'</td>
	<td>' .$day."-".$month."-".$year.'</td>
	<td>' .$gender.'</td>
	<td>' .$mobile_no.'</td>
	<td>' .$email_id.'</td>
	<td>' .$college_name.'</td>
	<td>' .$semester.'</td>
	<td>' .$usn.'</td></tr>';
echo '</table>';
echo '<h2>Your Photo</h2>';
$sql="select photo from user_data";
$result = $conn->query($sql);
$row=($result->fetch_assoc());
if ($result->num_rows > 0)
	'<img src="'.$row["photo"].'" alt="Mountain View" style="width:270px;height:250px;">';
$conn->close();
?>
