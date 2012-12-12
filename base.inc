<?php

abstract class EntityFieldQueryBase extends EntityFieldQuery {

  function execute() {
    $results = array();
    foreach (parent::execute() as $entity_type => $stubs) {
      $results[$entity_type] = entity_load($entity_type, array_keys($stubs));
    }
    return $results;
  }
}
