# WalidAmriou ESP IoT
Library of reusable components for Espressif IoT Development Framework

#### Folders
* HTTPS_server
* OTA_update_via_https
* WiFi
     * Connect_to_WiFi_Access_Point
     * Create_WiFi_Access_Point 

#### Development notes
- If you start a new Platformio project, use:
```bash
# put this line in platformio.ini :
monitor_speed = 115200 
# edit the CONFIG_ESP_HTTPS_SERVER_ENABLE line in sdkconfig to:
CONFIG_ESP_HTTPS_SERVER_ENABLE=y
# to fix could not open port /dev/ttyUSB0: [Errno 13] Permission denied: '/dev/ttyUSB0', open terminal and write:
sudo chmod a+rw /dev/ttyUSB0
```

### If you need any help or information:
:large_blue_circle:	 Facebook: https://www.facebook.com/walidamriou   
:large_blue_circle:  Twitter: https://twitter.com/walidamriou    
:large_blue_circle:  Linkedin: https://www.linkedin.com/in/walidamriou  
:red_circle: Email:  contact [at] walidamriou [dot] com    

### Copyright CC 2020 Walid Amriou

<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/">Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License</a>.

__You are free to:__
- __Share__ copy and redistribute the material in any medium or format

__Under the following terms:__
- __Attribution__ You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

- __NonCommercial__ You may not use the material for commercial purposes.

- __NoDerivatives__ If you remix, transform, or build upon the material, you may not distribute the modified material.

- __No additional restrictions__ You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.


the software or Code or Scripts or any files in this source is provided "as is" and the author disclaims all warranties with regard to this files including all implied warranties of merchantability and fitness. in no event shall the author be liable for any special, direct, indirect, or consequential damages or any damages whatsoever resulting from loss of use, data or profits, whether in an action of contract, negligence or other tortious action, arising out of or in connection with the use or performance of this software or code or scripts or any files in this source.

© The logo and the name and ideas of the project are owned by __Walid Amriou__, and do not fall under the open license for reconstruction, it must be preserved in full without modification. 
