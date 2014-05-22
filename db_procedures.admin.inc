<?php

function db_procedures_global_settings() {

	$form = array();

	
	$form['db_test'] = array(
		'#type' => 'submit',     
		'#value' => t('Test method'),
		'#submit' => array('db_procedures_db_create_procedure') ,
		);

	return system_settings_form($form);

	
	

}