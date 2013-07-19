<?php

abstract class EntityFieldQueryBase extends EntityFieldQuery {

  public static $entityType;

  function __construct($conf) {
    if (!empty(static::$entityType)) {
      $this->entityCondition('entity_type', static::$entityType);
      $bundles = $conf['bundles'];
      if (!empty($bundles[static::$entityType])) {
        $this->entityCondition('bundle', $conf['bundles'][static::$entityType]);
      }
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

    $form['bundles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bundles'),
      '#tree' => TRUE,
    );
    $entity_info = entity_get_info();
    foreach ($entity_info as $entity_type => $info) {

      // Temporarily only support one entity type.
      if ($entity_type != static::entityType()) {
        continue;
      }
      $options = array();
      foreach ($info['bundles'] as $bundle_name => $bundle_info) {
        $options[$bundle_name] = $bundle_info['label'];
      }
      $form['bundles'][$entity_type] = array(
        '#type' => 'checkboxes',
        '#title' => $info['label'],
        '#options' => $options,
        '#default_value' => isset($conf['bundles'][$entity_type]) ? isset($conf['bundles'][$entity_type]) : array(),
      );
    }

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
