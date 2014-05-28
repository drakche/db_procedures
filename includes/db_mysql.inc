<?php


function _db_create_procedure($name,
                              $param_input = FALSE,
                              $param_output = FALSE,
                              $param_inout = FALSE,
                              $code = FALSE,
                              $files = FALSE) {
//  if ($procedure_parameters_input_output_array) {
//    foreach ($procedure_parameters_input_output_array as $key => $value) {
//      $sql_query_pre_procedure = '';
//      $sql_query_pre_procedure .= 'SET ';
//      $sql_query_pre_procedure .= $key . '=' . $value . ';';
//
//      db_query($sql_query_pre_procedure);
//    }
//  }

  $sql_query = '';
  $sql_query .= 'CREATE PROCEDURE ' . $name;
  $sql_query .= '(';


  $joint_array = array();

  if ($param_input) {
    $array_temp = array();

    foreach ($param_input as $key => $value) {
      $tmp_string = 'IN ' . $key . ' ' . $value;
      array_push($array_temp, $tmp_string);
    }
    $in_param_string = join(', ', $array_temp);
    array_push($joint_array, $in_param_string);
  }
  if ($param_output) {
    $array_temp = array();

    foreach ($param_output as $key => $value) {
      $tmp_string = 'OUT ' . $key . ' ' . $value;
      array_push($array_temp, $tmp_string);
    }
    $in_param_string = join(', ', $array_temp);
    array_push($joint_array, $in_param_string);
  }
  if ($param_inout) {
    $array_temp = array();

    foreach ($param_inout as $key => $value) {
      $tmp_string = 'INOUT ' . $key . ' ' . $value;
      array_push($array_temp, $tmp_string);
    }
    $in_param_string = join(', ', $array_temp);
    array_push($joint_array, $in_param_string);
  }
  if (count($joint_array) > 0) {
    $sql_query .= join(', ', $joint_array);
  }
//  if ($procedure_parameters_output_array) {
//    $joint_array = array_merge($joint_array, $procedure_parameters_output_array);
//  }
//  if ($procedure_parameters_input_output_array) {
//    $joint_array = array_merge($joint_array, array_keys($procedure_parameters_input_output_array));
//  }
//  if (count($joint_array) > 0) {
//    $sql_query .= join(', ', $joint_array);
//  }

  $sql_query .= ')' . PHP_EOL;
  $sql_query .= 'BEGIN' . PHP_EOL;
  if ($code) {
    $sql_query .= $code . PHP_EOL;
  }
  if ($files) {
    foreach ($files as $key => $value) {
      $sql_from_file = file_get_contents(drupal_get_path('module', $key) . '/sql/' . $value);
      $sql_query .= $sql_from_file . PHP_EOL;
    }
  }
  $sql_query .= 'END';
//  dsm($sql_query);
  return db_query($sql_query);
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