pipeline{
    agent any

    environment{
        REPO_NAME = 'HTTP_Messenger_Server'
        LIBRARY_PATH = 'C:\\Users\\akork\\Desktop\\HttpMessengerServer\\vcpkg'
        LOGGER_PATH = 'C:\\Users\\akork\\Desktop\\HttpMessengerServer\\Logger'
        REQUIRED_FILES = 'C:\\Users\\akork\\Desktop\\HttpMessengerServer\\to_exe'
        
    }

    stages{
        stage('Preparing the directory'){
            steps{
                dir(env.REPO_NAME){
                    bat "echo '======================CHECKOUTING========================='"
                    cleanWs()
                    checkout scm
                }
            }
        }
        stage('Copying library to the project'){
            steps{
                dir(env.REPO_NAME){
                    bat "echo '======================COPYING LIBRARY TO THE DIRECTORY========================='"
                    bat "mkdir vcpkg"
                    bat "echo D | Xcopy ${env.LIBRARY_PATH} .\\vcpkg  /E /H /C /I"
                }
            }
        }
        stage('Copying logger to the project'){
            steps{
                dir(env.REPO_NAME){
                    bat "echo '======================COPYING LOGGER TO THE DIRECTORY========================='"
                    bat "mkdir Logger"
                    bat "echo D | Xcopy ${env.LOGGER_PATH} .\\Logger  /E /H /C /I"
                }
            }
        }
        stage('Build'){
            steps{
                dir(env.REPO_NAME){
                    bat "echo '======================BUILDING========================='"
                    bat "cmake --version"
                    bat "cmake . -B out"
                    bat "cmake --build out"
                }
            }
        }
        stage('Copying the file to the build'){
            steps{
                bat "echo '======================COPYING THE FILE========================='"
                bat "echo D | Xcopy ${env.REQUIRED_FILES} ${env.REPO_NAME}\\out\\Debug  /E /H /C /I"
            }
        }
    }
    post{
        success{
            script{
                archiveArtifacts(
                    artifacts: "/${env.REPO_NAME}/out/Debug/*.exe, /${env.REPO_NAME}/out/Debug/*.dll",
                    fingerprint: true
                )
            }
        }
    }
}
