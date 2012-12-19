<?php

$plugin = array(
  'title' => t("Node list"),
  'description' => t('List of nodes.'),
  'handler' => 'NodeList',
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

class NodeList extends EntityFieldQuery {

  function __construct($conf) {
    $this->entityCondition('entity_type', 'node');

    $bundles = array_filter($conf['bundles']);
    if (!empty($bundles)) {
      $this->entityCondition('bundle', array_filter($conf['bundles']), 'IN');
    }
    $this->propertyCondition('status', 1);
    $this->propertyOrderBy('sticky', 'asc');
    $this->propertyOrderBy('created', 'desc');
    if (!empty($conf['range']['start']) || !empty($conf['range']['length'])) {
      $this->range($conf['range']['start'], $conf['range']['length']);
    }
    $this->pager($conf['pager']['limit'], $conf['pager']['element']);
  }

  static function entityType() {
    return 'node';
  }

  static function editForm($form, &$form_state) {
    $form = array();
    $conf = $form_state['conf'];

    $entity_info = entity_get_info('node');

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
