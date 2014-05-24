<?php

function db_procedures_global_settings() {

	$form = array();

	
	$form['db_test'] = array(
		'#type' => 'submit',     
		'#value' => t('Test method'),
		'#submit' => array('tester') ,
		);

	return system_settings_form($form);

	
	

}