<?php


function _db_create_procedure($name,
                              $param_input = FALSE,
                              $param_output = FALSE,
                              $param_inout = FALSE,
                              $code = FALSE,
                              $files = FALSE) {

  $sql_query = sprintf('CREATE PROCEDURE %s(',$name);



  $joint_array = array();

  if ($param_input) {
    $array_temp = array();

    foreach ($param_input as $key => $value) {
//      $tmp_string = 'IN ' . $key . ' ' . $value;
      $tmp_string = sprintf('IN %s %s',$key,$value);
      array_push($array_temp, $tmp_string);
    }
    $in_param_string = join(', ', $array_temp);
    array_push($joint_array, $in_param_string);
  }
  if ($param_output) {
    $array_temp = array();

    foreach ($param_output as $key => $value) {
//      $tmp_string = 'OUT ' . $key . ' ' . $value;
      $tmp_string = sprintf('OUT %s %s',$key,$value);
      array_push($array_temp, $tmp_string);
    }
    $in_param_string = join(', ', $array_temp);
    array_push($joint_array, $in_param_string);
  }
  if ($param_inout) {
    $array_temp = array();

    foreach ($param_inout as $key => $value) {
//      $tmp_string = 'INOUT ' . $key . ' ' . $value;
      $tmp_string = sprintf('INOUT %s %s',$key,$value);
      array_push($array_temp, $tmp_string);
    }
    $in_param_string = join(', ', $array_temp);
    array_push($joint_array, $in_param_string);
  }
  if (count($joint_array) > 0) {
    $sql_query .= join(', ', $joint_array);
  }

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

function _db_alter_procedure($name,
                             $param_input = FALSE,
                             $param_output = FALSE,
                             $param_inout = FALSE,
                             $code = FALSE,
                             $files = FALSE){
  _db_drop_procedure($name);
  return _db_create_procedure($name,$param_input,$param_output,$param_inout,$code,$files);
}

function _db_drop_procedure($name) {
  $sql_query = sprintf('DROP PROCEDURE IF EXISTS %s;', $name);
//  dsm($sql_query);
  return db_query($sql_query);
}

function _db_execute_procedure($name,
                               $param_input = FALSE,
                               $param_output = FALSE,
                               $param_inout = FALSE) {

  if ($param_inout) {
    foreach ($param_inout as $key => $value) {
      $sql_query_pre_procedure = sprintf('SET %s = %s;',$key,$value);
//      $sql_query_pre_procedure .= 'SET ';
//      $sql_query_pre_procedure .= $key . '=' . $value . ';';

      db_query($sql_query_pre_procedure);
    }
  }

  $sql_query = sprintf('CALL %s(',$name);

//  $sql_query .= 'CALL ';
//  $sql_query .= $name;
//  $sql_query .= '(';
  $joint_array = array();
  if ($param_input) {
    $joint_array = array_merge($joint_array, $param_input);
  }
  if ($param_output) {
    $joint_array = array_merge($joint_array, $param_output);
  }
  if ($param_inout) {
    $joint_array = array_merge($joint_array, array_keys($param_inout));
  }
  if (count($joint_array) > 0) {
    $sql_query .= join(', ', $joint_array);
  }

  $sql_query .= ');';

  $result = db_query($sql_query);
  if ($param_output || $param_inout) {

    $sql_query_after_procedure = 'SELECT ';
    $joint_array = array();
    if ($param_output) {
      $joint_array = array_merge($joint_array, $param_output);
    }
    if ($param_inout) {
      $joint_array = array_merge($joint_array, array_keys($param_inout));
    }
    if (count($joint_array) > 0) {
      $sql_query_after_procedure .= join(', ', $joint_array);
    }
    $sql_query_after_procedure .= ';';
    // dsm($sql_query_after_procedure);
    $result = db_query($sql_query_after_procedure);

    // dsm($sql_query);
  }

  return $result;
}