<?php

$plugin = array(
  'title' => t("Node list"),
  'description' => t('List of nodes.'),
  'handler' => array(
    'class' => 'node_list',
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
