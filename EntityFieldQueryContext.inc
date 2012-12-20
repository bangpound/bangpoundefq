<?php

interface EntityFieldQueryContext {
  static function info(array $plugin, $parent);
  static function editForm($form, &$form_state);
}

/*
class BangpoundEFQContext implements EntityFieldQueryContext {
  protected $query;

  static function info(array $plugin, $parent) {
    $info['title'] = 'Node list';
    $info['keyword'] = 'node_list';
    $info['context name'] = 'node_list';
    $info['name'] = $parent . ':node_list';
    $info['description'] = t('Creates EFQ context from @class.', array('@class' => 'node_list'));
  }

  static function editForm($form, &$form_state) {
    $conf = $form_state['conf'];
    return $form;
  }
}
*/
