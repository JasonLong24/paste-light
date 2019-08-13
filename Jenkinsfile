pipeline {
  agent {
    docker { image 'gcc:8.3' }
  }
  stages {
    stage('clean') {
      steps {
        sh 'gcc --version'
        sh 'make clean'
      }
    }
    stage('Build') {
      steps {
        sh 'gcc --version'
        sh 'make -j'
      }
    }
    stage('Test') {
      steps {
        sh 'make tests'
        sh 'cd tests && ./test_paste_light'
      }
    }
  }
}
