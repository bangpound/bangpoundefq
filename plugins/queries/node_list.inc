<?php

$plugin = array(
  'title' => t("Node list"),
  'description' => t('List of nodes.'),
  'handler' => 'NodeList',
);

class NodeList extends EntityFieldQueryBase implements EntityFieldQueryContext, EntityFieldQueryContent {

  function __construct($conf) {
    $this->entityCondition('entity_type', 'node');
    $this->range($conf['range']['start'], $conf['range']['length']);
  }

  static function entityType() {
    return 'node';
  }

  static function info(array $plugin, $parent) {
    $info = $plugin;
    $info['title'] = 'Node list';
    $info['keyword'] = 'node_list';
    $info['context name'] = 'node_list';
    $info['name'] = $parent . ':node_list';
    $info['description'] = t('Creates EFQ context from @class.', array('@class' => 'node_list'));
    $info['defaults'] = array(
      'range' => array(
        'start' => NULL,
        'length' => NULL,
      ),
      'pager' => array(
        'limit' => '10',
        'element' => NULL,
      ),
    );
    return $info;
  }

  static function editForm($form, &$form_state) {
    $conf = $form_state['conf'];

    $form['range'] = array(
      '#type' => 'fieldset',
      '#title' => t('Range'),
      '#tree' => TRUE,
    );
    $form['range']['start'] = array(
      '#type' => 'textfield',
      '#title' => 'Start',
      '#default_value' => $conf['range']['start'],
    );
    $form['range']['length'] = array(
      '#type' => 'textfield',
      '#title' => 'Length',
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
      '#default_value' => $conf['pager']['limit'],
    );
    $form['pager']['element'] = array(
      '#type' => 'textfield',
      '#title' => 'Element',
      '#default_value' => $conf['pager']['element'],
    );
    return $form;
  }
}
