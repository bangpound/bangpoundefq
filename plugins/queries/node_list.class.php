<?php

class node_list extends EntityFieldQueryBase {

  public static $entityType = 'node';

  function __construct($conf) {
    parent::__construct($conf);

    $this->propertyCondition('status', 1);
    $this->propertyOrderBy('sticky', 'asc');
    $this->propertyOrderBy('created', 'desc');
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
