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
                    cleanWs()
                    checkout scm
                }
            }
        }
        stage('Copying library to the project'){
            steps{
                dir(env.REPO_NAME){
                    bat "mkdir vcpkg"
                    bat "echo D | Xcopy ${env.LIBRARY_PATH} .\\vcpkg"
                }
            }
        }
        stage('Build'){
            steps{
                dir(env.REPO_NAME){
                    bat "cmake --version"
                    bat "cmake CmakeLists.txt -B ./out"
                }
            }
        }
    }
    post{
        success{
            script{
                archiveArtifacts(
                    artifacts: "${env.REPO_NAME}/x64/Debug/*",
                    fingerprint: true
                )
            }
        }
    }
}
