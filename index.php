<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Data Sensor dan Galon</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap');
        * {
            font-family: 'Poppins';
        }
        table {
            width: 100%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black;
        }
        th, td {
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
    </style>
</head>
<body class="bg-white">
    <?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "matkul_iot";
    
    // Membuat koneksi
    $conn = new mysqli($servername, $username, $password, $dbname);
    
    // Memeriksa koneksi
    if ($conn->connect_error) {
        die("Koneksi gagal: " . $conn->connect_error);
    }
    ?>
    <header class="flex flex-row items-center w-[100%] h-[50px] bg-white border-b-[5px] border-blue-700">
        <img class="w-[30px] h-[30px] mx-[20px]" src="polindra.png" alt="">
        <div class="w-[100%] flex flex-row justify-between items-center pe-[50px]">
            <p class="text-[15px] font-bold me-[30px]">Smart Dispenser</p>
            <p class="text-[13px] w-[70px] py-[5px] rounded-full bg-blue-300 text-white text-center">admin</p>
        </div>
    </header>

    <div class="w-[80%] m-auto flex flex-col items-start mt-[10px]">
        

        <p class="text-[15px] font-bold mt-[50px]">Tabel Data</p>

        <div class="w-[100%] overflow-scroll h-[500px] rounded-[10px] border-[3px] border-b-[7px] border-blue-700 mt-[20px] p-[30px]">
            <table>
                <tr>
                    <th class="text-[13px]">ID</th>
                    <th class="text-[13px]">Jarak Datang</th>
                    <th class="text-[13px]">Status</th>
                    <th class="text-[13px]">Created At</th>
                </tr>
                <?php
                

                $sql = "SELECT id, jarak_datang, status, created_at FROM data_isi ORDER BY id DESC";
                $result = $conn->query($sql);

                if ($result->num_rows > 0) {
                    while($row = $result->fetch_assoc()) {
                        echo "<tr>
                                <td class='text-[13px]'>" . $row["id"]. "</td>
                                <td class='text-[13px]'>" . $row["jarak_datang"]. "</td>
                                <td class='text-[13px]'>" . $row["status"]. "</td>
                                <td class='text-[13px]'>" . $row["created_at"]. "</td>
                            </tr>";
                    }
                } else {
                    echo "<tr><td colspan='5' class='text-[13px]'>No data found</td></tr>";
                }
                ?>
            </table>
        </div>

       
    </div>


    
</body>
</html>
