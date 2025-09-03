Versions Used: Java 23, Maven 4.0.0, JUnit 5.8.2, JaCoCo-maven-plugin 0.8.7, Maven-surefire-plugin 2.22.2



1. We are using VS Code for this project. In order to use JaCoCo to generate the reports from the Junit test cases, we had to create a pom.xml file and insert it at the top level of our repository. This allowed us to use JaCoCo through a Maven framework, which is the pom.xml file. We have provided that file in our repository.


2. We kept Printtokens.java in src/main/java/printtokens and we kept PrinttokensUnitTest.java and PrinttokensE2ETest.java in src/test/java/printtokens. All of these files were included in a 'printtokens' package.


3. In order to get the coverage report for each test file, you need to keep only that file you are testing in the 'src/test/java/printtokens' directory. For example, if I wanted to test PrinttokensUnitTest.java on Printtokens.java, I would move PrinttokensE2ETest.java out of 'src/test/java/printtokens'.


4. Use the command 'mvn clean test' to run the test and generate the JaCoCo report. If you get an error, then you might need to run 'mvn compile' before that. If no errors show up, then the report should be successfully generated at the top level of the repository. You can access the report by using 'open target/site/jacoco/index.html' if you are on Mac, or any other way to open the file on your machine. You can also open the file from your file explorer at 'your-project/target/site/jacoco/index.html'.


5. You should now see the the html report in your browser with the coverages and other data collected from the JaCoCo report.