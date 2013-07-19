<?php

$plugin = array(
  'title' => t("Term list"),
  'description' => t('List of terms.'),
  'handler' => array(
    'class' => 'term_list',
  ),
  'defaults' => array(
    'bundles' => array(),
    'range' => array(
      'start' => NULL,
      'length' => NULL,
    ),
    'pager' => array(
      'limit' => '10',
      'element' => NULL,
    ),
  ),
);
