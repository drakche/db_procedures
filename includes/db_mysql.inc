<?php


function _db_create_procedure($procedure_name, $procedure_parameters_input_array = FALSE, $procedure_parameters_output_array = FALSE, $procedure_parameters_input_output_array = FALSE) {
  $sql_query = '';

  $sql_query .= "CREATE PROCEDURE ";


  dsm($sql_query);

}

function _db_execute_procedure($procedure_name,
                               $procedure_parameters_input_array = FALSE,
                               $procedure_parameters_output_array = FALSE,
                               $procedure_parameters_input_output_array = FALSE) {

  if ($procedure_parameters_input_output_array) {
    foreach ($procedure_parameters_input_output_array as $key => $value) {
      $sql_query_pre_procedure = '';
      $sql_query_pre_procedure .= 'SET ';
      $sql_query_pre_procedure .= $key . '=' . $value . ';';

      db_query($sql_query_pre_procedure);
    }
  }

  $sql_query = '';

  $sql_query .= 'CALL ';
  $sql_query .= $procedure_name;
  $sql_query .= '(';
  $joint_array = array();
  if ($procedure_parameters_input_array) {
    $joint_array = array_merge($joint_array, $procedure_parameters_input_array);
  }
  if ($procedure_parameters_output_array) {
    $joint_array = array_merge($joint_array, $procedure_parameters_output_array);
  }
  if ($procedure_parameters_input_output_array) {
    $joint_array = array_merge($joint_array, array_keys($procedure_parameters_input_output_array));
  }
  if (count($joint_array) > 0) {
    $sql_query .= join(', ', $joint_array);
  }

  $sql_query .= ')';
  $sql_query .= ';';

  $result = db_query($sql_query);
  if ($procedure_parameters_output_array || $procedure_parameters_input_output_array) {

    $sql_query_after_procedure = '';
    $sql_query_after_procedure .= 'SELECT ';
    $joint_array = array();
    if ($procedure_parameters_output_array) {
      $joint_array = array_merge($joint_array, $procedure_parameters_output_array);
    }
    if ($procedure_parameters_input_output_array) {
      $joint_array = array_merge($joint_array, array_keys($procedure_parameters_input_output_array));
    }
    if (count($joint_array) > 0) {
      $sql_query_after_procedure .= join(', ', $joint_array);
    }
    $sql_query_after_procedure .= ';';
    // dsm($sql_query_after_procedure);
    return db_query($sql_query_after_procedure);

    // dsm($sql_query);
  }

  return $result;
}