<?php 
exec('/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport -s | awk -F \':\' \'{print $1}\' | sed \'s/..$//\' | tail -n +2', $host_wifi_ssid);
foreach (array_unique($host_wifi_ssid) as $ssid){
    echo "<option>" . $ssid . "</option>";  
}
?>
