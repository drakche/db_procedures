<?php


function db_create_procedure($procedure_name, $procedure_parameters_input_array=NULL, $procedure_parameters_output_array=NULL){
	$sql_query='';

	$sql_query=$sql_query."CREATE PROCEDURE SQLITE";


	dsm($sql_query);

}