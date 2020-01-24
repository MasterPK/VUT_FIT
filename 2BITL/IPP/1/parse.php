<?php
/* IPP project - parse.php
 * Author: Petr Křehlík (xkrehl04)
 * Date: 12/3/2019
*/

$stdin = fopen("php://stdin", "r");
$stderr = fopen('php://stderr', 'w');


//Debug function, writes on stderr
$debug_enable=false;
function debug($string)
{
	global $stderr;
	global $debug_enable;
	if($debug_enable==true)
	{
		fwrite($stderr,$string);
		fwrite($stderr,"\n");
	}
	
}

// Error function, writes on stderr
function error($code,$line,$msg)
{
	global $stderr;
	fwrite($stderr, "Error ".$code." on line ".$line.": ".$msg."\n");
	exit($code);
}


// Arguments
$longopts  = array(
    "help::",
);
$arguments = getopt("", $longopts);

if(array_key_exists("help",$arguments))
{
	$help="
Skript typu filtr (parse.php v jazyce PHP 7.3) nacte ze standardniho vstupu zdrojovy kod v IPPcode19, zkontroluje lexikalni a syntaktickou spravnost kodu a vypise na standardni vystup XML.

Priklad pouziti: php7.3 parse.php [--help] <in.src >out.xml
";
	echo $help;
	exit(0);
}

// List of instructions

$instructions=array(
	"MOVE" => array("var","symb"),
	"CREATEFRAME" => array(),
	"PUSHFRAME" => array(),
	"POPFRAME" => array(),
	"DEFVAR" => array("var"),
	"CALL" => array("label"),
	"RETURN" => array(),
	"PUSHS" => array("symb"),
	"POPS" => array("var"),
	"ADD" => array("var","symb","symb"),
	"SUB" => array("var","symb","symb"),
	"MUL" => array("var","symb","symb"),
	"IDIV" => array("var","symb","symb"),
	"LT" => array("var","symb","symb"),
	"GT" => array("var","symb","symb"),
	"EQ" => array("var","symb","symb"),
	"AND" => array("var","symb","symb"),
	"OR" => array("var","symb","symb"),
	"NOT" => array("var","symb"),
	"INT2CHAR" => array("var","symb"),
	"STRI2INT" => array("var","symb","symb"),
	"READ" => array("var","type"),
	"WRITE" => array("symb"),
	"CONCAT" => array("var","symb","symb"),
	"STRLEN" => array("var","symb"),
	"GETCHAR" => array("var","symb","symb"),
	"SETCHAR" => array("var","symb","symb"),
	"TYPE" => array("var","symb"),
	"LABEL" => array("label"),
	"JUMP" => array("label"),
	"JUMPIFEQ" => array("label","symb","symb"),
	"JUMPIFNEQ" => array("label","symb","symb"),
	"EXIT" => array("symb"),
	"DPRINT" => array("symb"),
	"BREAK" => array()
);

$string_regexp="/^string@([^\s#\\\\]|\\\\[0-9]{3})*$/";
$bool_regexp="/^bool@(true|false)$/";
$int_regexp="/^int@[+|-]?[0-9]+$/";
$nil_regexp="/^nil@nil$/";
$var_regexp="/^(GF|LF|TF)@([a-zA-Z]|[_!?*%$&-]\w)([a-zA-Z0-9]|[_!?*%$&-])*$/";
$type_regexp="/^(int|bool|string)$/";
$label_regexp="/^([a-zA-Z]|[_!?*%$&-]\w)([a-zA-Z0-9]|[_!?*%$&-])*$/";


// XML header

$xml = xmlwriter_open_memory();
xmlwriter_set_indent($xml, 1);
xmlwriter_set_indent_string($xml, '    ');
xmlwriter_start_document($xml, '1.0', 'UTF-8');

xmlwriter_start_element($xml, 'program');
xmlwriter_start_attribute($xml, 'language');
xmlwriter_text($xml, 'IPPcode19');
xmlwriter_end_attribute($xml);

$line_count=1;

// Cycle that search for init line

$found_start=false;
while (($line=fgets($stdin)) != false) 
{

	// Remove extra white chars
	
	$line=trim($line);
	$line = preg_replace("/\s+/", " ", $line);

	// Skip empty line

	if(empty($line))
	{
		continue;
	}

	// If alone comment, skip

	$comments_pos=strpos($line, "#");
	if($comments_pos===0)
	{
		continue;
	}
	elseif ($comments_pos>0) 
	{
		// if is something before comment, then explode

		$line=explode("#",$line)[0];
	}

	// Again remove extra white chars

	$line=trim($line);

	// Normalize, to lowercase

	$line=strtolower($line);

	// Test for correct format

	if($line==".ippcode19")
	{
		$line_count++;
		$found_start=true;
		break;
	}
	else
	{
		error(21,$line_count,"Wrong first line format!");
	}

	$line_count++;
}

if(!$found_start)
{
	error(21,$line_count,"Not found first line!");
}

// Main cycle that check syntax

while (($line=fgets($stdin)) != false) 
{
	
	
	$line=trim($line);
	$line = preg_replace("/\s+/", " ", $line);
	if(empty($line))
	{
		continue;
	}

	$comments_pos=strpos($line, "#");
	if($comments_pos===0)
	{
		continue;
	}
	elseif ($comments_pos>0) 
	{
		$line=explode("#",$line)[0];
	}
	$line=trim($line);
	
	$split_str=explode(" ",$line);
	$split_str[0]=strtoupper($split_str[0]);

	// Check if instruction is valid

	if(array_key_exists($split_str[0], $instructions)==false)
	{
		error(22,$line_count,"Wrong instruction code!");
	}

	xmlwriter_start_element($xml, 'instruction');
	xmlwriter_start_attribute($xml, 'order');
	xmlwriter_text($xml, $line_count-1);
	xmlwriter_end_attribute($xml);
	xmlwriter_start_attribute($xml, 'opcode');
	xmlwriter_text($xml, $split_str[0]);
	xmlwriter_end_attribute($xml);

	// Instruction without operands

	if(empty($instructions[$split_str[0]]))
	{
		xmlwriter_end_element($xml);
		$line_count++;
		continue;
	}

	$i=0;
	
	// Check if count of operands is fine

	if((count($split_str)-1)!=count($instructions[$split_str[0]]))
	{
		error(23,$line_count,"Wrong arguments count!");
	}

	// CCheck if syntax of operands is fine

	foreach ($instructions[$split_str[0]] as $argument) 
	{
		++$i;
		$arg_name= "arg".$i;
		xmlwriter_start_element($xml,$arg_name);
		xmlwriter_start_attribute($xml, 'type');
		if($argument=="var")
		{
			if(preg_match($var_regexp, $split_str[$i]))
			{
				xmlwriter_text($xml, "var");
			}
			else
			{
				error(23,$line_count,"Wrong variable format!");
			}
		}

		if($argument=="symb")
		{
			if(preg_match($string_regexp, $split_str[$i]))
			{
				$split_str[$i]=explode("@",$split_str[$i],2)[1];
				xmlwriter_text($xml, "string");
			}
			elseif (preg_match($bool_regexp, $split_str[$i])) 
			{
				$split_str[$i]=explode("@",$split_str[$i],2)[1];
				xmlwriter_text($xml, "bool");
			}
			elseif (preg_match($int_regexp, $split_str[$i])) 
			{
				$split_str[$i]=explode("@",$split_str[$i],2)[1];
				xmlwriter_text($xml, "int");
			}
			elseif (preg_match($nil_regexp, $split_str[$i])) 
			{
				$split_str[$i]=explode("@",$split_str[$i],2)[1];
				xmlwriter_text($xml, "nil");
			}
			elseif (preg_match($var_regexp, $split_str[$i])) 
			{
				xmlwriter_text($xml, "var");
			}
			else
			{
				error(23,$line_count,"Wrong symbol format!");
			}
		}

		if($argument=="type")
		{
			if(preg_match($type_regexp, $split_str[$i]))
			{
				xmlwriter_text($xml, "type");
			}
			else
			{
				error(23,$line_count,"Wrong type format!");
			}
		}

		if($argument=="label")
		{
			if(preg_match($label_regexp, $split_str[$i]))
			{
				xmlwriter_text($xml, "label");
			}
			else
			{
				error(23,$line_count,"Wrong label format!");
			}
			
		}

		

		xmlwriter_end_attribute($xml);
		xmlwriter_text($xml, $split_str[$i]);
		xmlwriter_end_element($xml);
	}
	xmlwriter_end_element($xml);
	$line_count++;

}
// Finally print output

xmlwriter_end_element($xml);
xmlwriter_end_document($xml);
echo xmlwriter_output_memory($xml);


?>