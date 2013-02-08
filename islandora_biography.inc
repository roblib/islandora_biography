<?php

class DisplayBiography {

  private $pid = NULL;

  function DisplayBiography($pid) {
    $this->pid = $pid;
  }

  function showBiography() {
    global $base_url;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new fedora_item($this->pid);
    $title = $item->objectProfile->objLabel;
    drupal_add_css(drupal_get_path('module', 'islandora_biography') . '/css/islandora_biography.css');
    $modsString = $item->get_datastream_dissemination('MODS');
    $modsXml = new SimpleXMLElement($modsString);
    $modsXml->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3");
    $strings = $this->load_strings($modsXml);
    $img_text = "<img src = $base_url/fedora/repository/$this->pid/TN/TN>";
    $img = l($img_text, "$base_url/fedora/repository/$this->pid/OBJ/OBJ", array('html' => TRUE));
    $guide = l(t('Guide to abbreviations'), 'http://vre2.upei.ca/leg/abbreviations');
    $honourable = (empty($strings['honourable'])) ? "" : $strings['honourable'];
    $html = "<div class = 'bio-title'>$honourable[0] " . $strings['first_name'][0] . " " . $strings['last_name'][0] . "</div><br />";
    $html .='<div class = "bio-sitting">' . $strings['sitting_dates'][0] . '</div><br />';
    $html .='<div class="bio-img">'.$img.'</div>';
    $html .= '<div class = "bio-body">' . $strings['bio_desc'][0] . '<p>' . 'References: ' . $strings['references'][0] . ' '. $guide .'</p>';
    if(!empty($strings['paro_number'][0])  && strlen($strings['paro_number'][0]) > 3){
      $html .= '<div class = "bio-ack">Photograph courtesy of PEI Public Archives and Records Office, 
      Reference Number ' . $strings['paro_number'][0] .'</div>';
    } else {
      $html .= "<div class = 'bio-ack'>We currently do not have an image of this Member in the collection. <a href = '/node/26'>Can you help</a>?</div>";
    }
    $collection_fieldset = array(
      '#collapsible' => FALSE,
      '#value' => $html,
    );
    $tabset = array();
    $tabset['image'] = array(
      '#type' => 'tabpage',
      '#title' => $title,
      '#content' => theme('fieldset', $collection_fieldset),
    );

    return $tabset;
  }

  function load_strings($modsXml) {
    $strings = array();
    $strings['title'] = $modsXml->xpath('/mods:mods/mods:titleInfo/mods:title');
    $strings['honourable'] = $modsXml->xpath('/mods:mods/mods:subject/mods:name/mods:namePart[@type="termsOfAddress"]');
    $strings['first_name'] = $modsXml->xpath('/mods:mods/mods:subject/mods:name/mods:namePart[@type="given"]');
    $strings['last_name'] = $modsXml->xpath('/mods:mods/mods:subject/mods:name/mods:namePart[@type="family"]');
    $strings['sitting_dates'] = $modsXml->xpath('/mods:mods/mods:subject/mods:name/mods:description');
    $strings['bio_desc'] = $modsXml->xpath('/mods:mods/mods:note[@type="biography"]');
    $strings['references'] = $modsXml->xpath('//mods:note[@type="references"]');
    $strings['paro_number'] = $modsXml->xpath('//mods:identifier[@type="paro"]');
    return $strings;
  }

}
