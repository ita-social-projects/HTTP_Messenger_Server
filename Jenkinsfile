pipeline{
    agent any

    environment{
        REPO_NAME = 'HTTP_Messenger_Server'
        LIBRARY_PATH = ''
    }

    stages{
        stage('Preparing the directory'){
            steps{
                dir(env.REPO_NAME){
                    cleanWs()
                    checkout scm
                }
            }
        }
        stage('Copying library to the project'){
            steps{
                dir(env.REPO_NAME){
                    bat "mkdir vcpkg"
                    bat "copy ${env.LIBRARY_PATH} vcpkg"
                }
            }
        }
        stage('Copying library to the project'){
            steps{
                dir(env.REPO_NAME){
                    bat "mkdir vcpkg"
                    bat "copy ${env.LIBRARY_PATH} vcpkg"
                }
            }
        }
        stage('Build'){
            steps{
                dir(env.REPO_NAME){
                    bat ""
                }
            }
        }
    }
}
"""git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install cpprestsdk"""