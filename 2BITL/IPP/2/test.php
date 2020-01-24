<?php
/* IPP project - test.php
 * Author: Petr Křehlík (xkrehl04)
 * Date: 10/4/2019
*/
error_reporting(E_ERROR | E_PARSE);

//Debug function, writes on stderr
$stderr = fopen('php://stderr', 'w');

// Error function, writes on stderr
function error($code,$msg)
{
	global $stderr;
	fwrite($stderr, "Error ".$code.": ".$msg."\n");
	exit($code);
}


// Arguments
$longopts  = array(
    "help::",
    "directory::",
    "recursive::",
    "parse-script::",
    "int-script::",
    "parse-only::",
    "int-only::"
);
$arguments = getopt("", $longopts);

if(array_key_exists("help",$arguments))
{
	$help="
    Skript projde zadaný adresář s testy a využije je pro automatické
    otestování správné funkčnosti obou předchozích programů včetně vygenerování přehledného souhrnu
    v HTML 5 do standardního výstupu.

Priklad pouziti: php7.3 test.php [--help] [--directory] [--recursive] [--parse-script] [--int-script] [--parse-only] [--int-only] 
";
	echo $help;
	exit(0);
}

$parse_script="parse.php";
$int_script="interpret.py";
$directory=".";
$recursive=false;
$parse_only=false;
$int_only=false;

#Test arguments

if($int_only == true && $parse_only == true)
{
    error(10,"Cant use int-only and parse-only at same time!");
}

if(array_key_exists("directory",$arguments))
{

    $directory=$arguments["directory"];
    
}

# Main HTML code
echo"<!doctype html>

<html lang=\"cs\">
<head>
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">

    <title>Výsledky testů</title>
    <meta name=\"description\" content=\"Výsledky testů IPP - test.php\">
    <meta name=\"author\" content=\"Petr Krehlik (xkrehl04)\">
    <style>
    .green {
        color: green;
    }
    .red {
        color: red;
    }
    table{
        width:100%;
    }
    th{
        width:20%;
        border:none;
        background-color:#f1cab5;
        height:40px;
    }
    td{ 
        width:20%;
        border:none;
        text-align:center;
        padding:10px;
        background-color:#e3e3e3;
    }

    </style>

</head>

<body>
";

if(array_key_exists("parse-script",$arguments))
{
    
    $parse_script=$arguments["parse-script"];
}

if(array_key_exists("int-script",$arguments))
{
    
    $int_script=$arguments["int-script"];
}

if(array_key_exists("parse-only",$arguments))
{
    echo "<h2>Výsledky testů skriptu parse.php</h2>";
    echo "Bylo použito PHP verze 7.3 a XML výstupy byly porovnávány pomocí programu JExamXML. ";
    $parse_only=true;
}

if(array_key_exists("int-only",$arguments))
{
    echo "<h2>Výsledky testů skriptu interpret.py</h2>";
    echo "Byl použit Python verze 3.6 a výstupy byly porovnávány pomocí programu diff. ";
    $int_only=true;
}

if(array_key_exists("parse-only",$arguments)==false && array_key_exists("int-only",$arguments)==false)
{
    echo "<h2>Výsledky postupného testování skriptů parse.php a interpret.py</h2>";
    echo "Bylo použito PHP verze 7.3 a Python verze 3.6. XML výstupy byly porovnávány pomocí programu JExamXML a ostatní výpisy pomocí diff. ";
}

if(array_key_exists("recursive",$arguments))
{
    $recursive=true;
    echo "Prohledávaná složka byla prohledána <b>včetně</b> podsložek.";
}
else
{
    echo "Podsložky <b>nebyly</b> prohledávány.";
}

# Header of table
echo "<table>
<tr>
<th>Číslo</th>
<th>Název</th>
<th>Očekávaný návratový kód</th>
<th>Skutečný návratový kód</th>
<th>Výsledek</th>
";

$total_tests=0;
$passed_tests=0;

# Determine recursive iterator
if($recursive)
{
    $iterator=new RecursiveIteratorIterator(new RecursiveDirectoryIterator($directory, RecursiveDirectoryIterator::SKIP_DOTS),RecursiveIteratorIterator::SELF_FIRST);
}
else
{
    $iterator=new DirectoryIterator($directory);
}
# Loop through files
foreach ($iterator as $file) {
    # skip whatewher is not .src
    if(!$recursive) {if($file->isDot()) continue;}
    if($file->isDir()) continue;
    if($file->getExtension()!="src") continue;
    
    $file_without_extension=$file->getPath()."/".$file->getBasename(".src");

    $rc_expected=0;
    $rc_filename=$file_without_extension.".rc";

    $rc_file= file_get_contents($rc_filename);
    if($rc_file===false)
    {
        $rc_file = fopen($rc_filename, "w");
        $txt = "0\n";
        fwrite($rc_file, $txt);
        fclose($rc_file);
    }
    else
    {
        $rc_expected=trim($rc_file);
    }

    $in_filename=$file_without_extension.".in";
    $in_file= file_exists($in_filename);
    if($in_file===false)
    {
        $in_file = fopen($in_filename, "w");
        fwrite($in_file, "");
        fclose($in_file);
        $in_file="";
    }

    $out_filename=$file_without_extension.".out";
    $out_file= file_exists($out_filename);
    if($out_file===false)
    {
        $out_file = fopen($out_filename, "w");
        fwrite($out_file, "");
        fclose($out_file);
        $out_file="";
    }

    $total_tests++;
    
    if($parse_only)
    {
        if(file_exists($parse_script)==false) error(11,"Cannot find parse.php script!");
        $rc_actual=trim(shell_exec("php7.3 ".$parse_script." <".$file_without_extension.".src >".$file_without_extension.".out_actual; echo $?"));
        echo "
        <tr>
            <td>".$total_tests."</td>
            <td>".$file->getFileInfo()."</td>
            <td>".$rc_expected."</td>
            <td>".$rc_actual."</td>
        ";
        
        if($rc_actual==$rc_expected)
        {
            if($rc_actual=="0")
            {
                $rc_examxml=trim(shell_exec("java -jar /pub/courses/ipp/jexamxml/jexamxml.jar ".$file_without_extension.".out ".$file_without_extension.".out_actual delta.xml /pub/courses/ipp/jexamxml/options >/dev/null;echo $?"));

                if($rc_examxml=="0")
                {
                    echo "<td><b class=green>OK</b></td>";
                    $passed_tests++;
                }
                else
                {
                    echo "<td><b class=red>FAIL</b></td>";
                }
            }
            else
            {
                echo "<td><b class=green>OK</b></td>";
                $passed_tests++;
            }
            
        }
        else
        {
            echo "<td><b class=red>FAIL</b></td>";
        }
        echo
        "</tr>";
        shell_exec("rm ".$file_without_extension.".out_actual");
    }

    if($int_only)
    {

        if(file_exists($int_script)==false) error(11,"Cannot find interpret.py script!");
        $rc_actual=trim(shell_exec("python3.6 ".$int_script." --source=".$file_without_extension.".src --input=".$file_without_extension.".in >".$file_without_extension.".out_actual; echo $?"));
        echo "
        <tr>
            <td>".$total_tests."</td>
            <td>".$file->getFileInfo()."</td>
            <td>".$rc_expected."</td>
            <td>".$rc_actual."</td>
        ";
        if($rc_actual==$rc_expected)
        {
            if($rc_actual=="0")
            {
                $rc_diff=trim(shell_exec("diff ".$file_without_extension.".out ".$file_without_extension.".out_actual; echo $?"));
                shell_exec("rm ".$file_without_extension.".out_actual");
                if($rc_diff=="0")
                {
                    echo "<td><b class=green>OK</b></td>";
                    $passed_tests++;
                }
                else
                {
                    echo "<td><b class=red>FAIL</b></td>";
                }
            }
            else
            {
                echo "<td><b class=green>OK</b></td>";
                $passed_tests++;
            }
        }
        else
        {
            echo "<td><b class=red>FAIL</b></td>";
        }
        echo
        "</tr>";
        
    }

    if($int_only==false && $parse_only==false)
    {
        if(file_exists($parse_script)==false) error(11,"Cannot find parse.php script!");
        if(file_exists($int_script)==false) error(11,"Cannot find interpret.py script!");
        $rc_parse=trim(shell_exec("php7.3 ".$parse_script." <".$file_without_extension.".src >".$file_without_extension.".out_parse; echo $?"));
        echo "
        <tr>
            <td>".$total_tests."</td>
            <td>".$file->getFileInfo()."</td>
            <td>".$rc_expected."</td>
        ";
        if($rc_parse !="0")
        {
            shell_exec("rm ".$file_without_extension.".out_parse");
            echo "<td>".$rc_parse."</td>";
            if($rc_parse==$rc_expected)
            {
                echo "<td><b class=green>OK</b></td>";
                $passed_tests++;
            }
            else
            {
                echo "<td><b class=red>FAIL</b></td>";
            }
            continue;
        }
        $rc_int=trim(shell_exec("python3.6 ".$int_script." --source=".$file_without_extension.".out_parse --input=".$file_without_extension.".in >".$file_without_extension.".out_actual; echo $?"));
        echo "<td>".$rc_int."</td>";
        shell_exec("rm ".$file_without_extension.".out_parse");
        if($rc_int==$rc_expected)
        {
            if($rc_int=="0")
            {
                $rc_diff=trim(shell_exec("diff ".$file_without_extension.".out ".$file_without_extension.".out_actual >/dev/null; echo $?"));
                
                
                if($rc_diff=="0")
                {
                    echo "<td><b class=green>OK</b></td>";
                    $passed_tests++;
                }
                else
                {
                    echo "<td><b class=red>FAIL</b></td>";
                }
            }
            else
            {
                echo "<td><b class=green>OK</b></td>";
                $passed_tests++;
            }
           
            
        }
        else
        {
            echo "<td><b class=red>FAIL</b></td>";
        }
        echo
        "</tr>";
        shell_exec("rm ".$file_without_extension.".out_actual");
    }
    

    
}
echo "</table>";

echo "<h3>Úspěšnost testů: ".$passed_tests."/".$total_tests."</h3>";
echo "
</body>
</html>";

?>