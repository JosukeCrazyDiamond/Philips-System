WT-Philip-Hue Project

Stage 2 Compile command:
`c++ -o wt-philip-hue *.cpp -lwthttp -lwt -lwtdbo -lwtdbosqlite3 -lboost_system -std=c++0x`
Start WT Server:
`./wt-philip-hue --docroot /usr/local/share/Wt --http-address 0.0.0.0 --http-port 8080`

### What have been accomplished:
##### Login function:
The project currently supports user registration and login. First time user can register by providing first name, last name, email (as login username) and password (with password confirmation).
Registered login information are stored using WT build-in DB combined with WT build-in Authentication.
The user credentials are stored separately in two database tables:

`auth-info` table which stores hashed password and user unique id.

`user` table stores user general information, currently contains first and last name.

##### Bridge:
The project currently supports Bridge creation, update and deletion.
Bridges are associated users in a per user base, meaning each bridge are associated with a user id.
The created bridges data are stored in WT build-in DB to achieve data persistence. Although it may not be reflected at UI in current stage, this will be implemented as project being finished.

#### Means to verify login credential & Bridge creation/modification.
The WT build-in DB stores data into a `.db` file. As the project launches, it will be generated at `wt-philip-hue.db` at root level of the project.
The persistent data storage can be verifies currently through inspecting this file by various means.
For example, It can be loaded through a online db viewer site such as: [https://sqliteonline.com/](https://sqliteonline.com/) 

####Project Structure:
Bridge, BridgeWidget supports Bridge functionality.

User, UserDetailsModel, ExtendedRegistrationWidget, ExtendedAuthWidget supports Login functionality.

Session supports DB connection as well as maintains a unique user login session with the server.

UserInterfaceWidget provides the UI after User login
