pipeline {
  agent {
    docker { image 'gcc:8.3' }
  }
  stages {
    stage('Build') {
      steps {
        sh 'gcc --version'
        sh 'make -j'
      }
    }
    stage('Install') {
      steps {
        sh 'sudo make install'
      }
    }
    stage('Test') {
      steps {
        sh 'make tests'
        sh 'cd tests && ./tests/test_paste_light'
      }
    }
  }
}
