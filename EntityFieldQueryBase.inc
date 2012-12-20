<?php

abstract class EntityFieldQueryBase extends EntityFieldQuery {

  public static $entityType;

  function __construct($conf) {
    $bundles = array_filter($conf['bundles']);
    if (!empty(static::$entityType)) {
      $this->entityCondition('entity_type', static::$entityType);
    }
    if (!empty($bundles)) {
      $this->entityCondition('bundle', array_filter($conf['bundles']), 'IN');
    }
    if (!empty($conf['range']['start']) || !empty($conf['range']['length'])) {
      $this->range($conf['range']['start'], $conf['range']['length']);
    }
    $this->pager($conf['pager']['limit'], $conf['pager']['element']);
  }

  static function entityType() {
    return static::$entityType;
  }

  static function editForm($form, &$form_state) {
    $conf = $form_state['conf'];

    $entity_info = entity_get_info(static::$entityType);

    $options = array();
    foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
      $options[$bundle_name] = $bundle_info['label'];
    }
    $form['bundles'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#title' => t('Bundles'),
      '#default_value' => $conf['bundles'],
    );

    $form['range'] = array(
      '#type' => 'fieldset',
      '#title' => t('Range'),
      '#tree' => TRUE,
    );
    $form['range']['start'] = array(
      '#type' => 'textfield',
      '#title' => 'Start',
      '#size' => 10,
      '#default_value' => $conf['range']['start'],
    );
    $form['range']['length'] = array(
      '#type' => 'textfield',
      '#title' => 'Length',
      '#size' => 10,
      '#default_value' => $conf['range']['length'],
    );
    $form['pager'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pager'),
      '#tree' => TRUE,
    );
    $form['pager']['limit'] = array(
      '#type' => 'textfield',
      '#title' => 'Limit',
      '#size' => 10,
      '#default_value' => $conf['pager']['limit'],
    );
    $form['pager']['element'] = array(
      '#type' => 'textfield',
      '#title' => 'Element',
      '#size' => 10,
      '#default_value' => $conf['pager']['element'],
    );

    return $form;
  }

  static function validateForm(&$form, &$form_state) {
    if (empty($form_state['values']['range']['start'])) {
      form_set_value($form['range']['start'], NULL, $form_state);
    }
    if (empty($form_state['values']['range']['length'])) {
      form_set_value($form['range']['length'], NULL, $form_state);
    }
  }
}
