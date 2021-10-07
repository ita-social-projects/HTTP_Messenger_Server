pipeline{
    agent any

    environment{
        REPO_NAME = 'HTTP_Messenger_Server'
        LIBRARY_PATH = 'C:\\Users\\akork\\Desktop\\HttpMessengerServer\\vcpkg'
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
    }
    post{
        success{
            script{
                archiveArtifacts(
                    artifacts: "${env.REPO_NAME}/out/debug/*",
                    fingerprint: true
                )
            }
        }
    }
}
