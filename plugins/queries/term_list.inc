<?php

$plugin = array(
  'title' => t("Term list"),
  'description' => t('List of terms.'),
  'handler' => 'TermList',
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

class TermList extends EntityFieldQueryBase {

  public static $entityType = 'taxonomy_term';

  function __construct($conf) {
    parent::__construct($conf);
  }

  static function editForm($form, &$form_state) {
    $form = parent::editForm($form, $form_state);
    $conf = $form_state['conf'];
    return $form;
  }

  static function validateForm(&$form, &$form_state) {
    parent::validateForm($form, $form_state);
  }
}
