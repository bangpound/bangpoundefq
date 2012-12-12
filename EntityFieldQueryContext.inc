<?php

interface EntityFieldQueryContext {
  static function info(array $plugin, $parent);
  static function editForm($form, &$form_state);
}

interface EntityFieldQueryContent {
  static function info(array $plugin, $parent);
  static function editForm($form, &$form_state);
  static function entityType();
}
