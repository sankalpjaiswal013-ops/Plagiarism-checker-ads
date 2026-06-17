const RESUME_CORPUS = [
  {
    name: "Introduction to Algorithms",
    text: "An algorithm is any well-defined computational procedure that takes some value or set of values as input and produces some value or set of values as output. An algorithm is thus a sequence of computational steps that transform the input into the output. We can also view an algorithm as a tool for solving a well-specified computational problem. The statement of the problem specifies in general terms the desired input/output relationship. The algorithm describes a specific computational procedure for achieving that input/output relationship. Algorithms are used in nearly every branch of computing and in many fields outside of computing."
  },
  {
    name: "Data Structures - Abstract Data Types",
    text: "A data structure is a way of organizing data in a computer so that it can be accessed and modified efficiently. Different data structures are suited to different kinds of applications and some are highly specialized to specific tasks. Data structures provide a means to manage large amounts of data efficiently for uses such as large databases and internet indexing services. The choice of a data structure often begins from the choice of an abstract data type. A well-designed data structure allows a variety of critical operations to be performed using as few resources, both execution time and memory space, as possible."
  },
  {
    name: "Operating Systems Fundamentals",
    text: "An operating system is system software that manages computer hardware, software resources, and provides common services for computer programs. The operating system is a component of the system software in a computer system. Time-sharing operating systems schedule tasks for efficient use of the system and may also include accounting software for cost allocation of processor time, mass storage, printing, and other resources. For hardware functions such as input and output and memory allocation, the operating system acts as an intermediary between programs and the computer hardware. Operating systems are found on many devices that contain a computer, from cellular phones and video game consoles to web servers and supercomputers."
  },
  {
    name: "Computer Networks Overview",
    text: "A computer network is a set of computers sharing resources located on or provided by network nodes. The computers use common communication protocols over digital interconnections to communicate with each other. These interconnections are made up of telecommunication network technologies, based on physically wired, optical, and wireless radio-frequency methods that may be arranged in a variety of network topologies. The nodes of a computer network can include personal computers, servers, networking hardware, or other specialized or general-purpose hosts. They are identified by network addresses, and may have hostnames."
  },
  {
    name: "Database Systems and DBMS",
    text: "A database is an organized collection of structured information or data, typically stored electronically in a computer system. A database is usually controlled by a database management system. Together, the data and the DBMS, along with the applications that are associated with them, are referred to as a database system, often shortened to just database. Data within the most common types of databases in operation today is typically modeled in rows and columns in a series of tables to make processing and data querying efficient. The data can then be easily accessed, managed, modified, updated, controlled, and organized."
  },
  {
    name: "Machine Learning Basics",
    text: "Machine learning is a method of data analysis that automates analytical model building. It is based on the idea that systems can learn from data, identify patterns, and make decisions with minimal human intervention. Machine learning is a type of artificial intelligence that allows software applications to become more accurate at predicting outcomes without being explicitly programmed to do so. Machine learning algorithms use historical data as input to predict new output values. Recommendation engines are a common use case for machine learning. Other popular uses include fraud detection, spam filtering, malware threat detection, and business process automation."
  },
  {
    name: "Cybersecurity Principles",
    text: "Cybersecurity is the practice of protecting systems, networks, and programs from digital attacks. These cyberattacks are usually aimed at accessing, changing, or destroying sensitive information; extorting money from users; or interrupting normal business processes. Implementing effective cybersecurity measures is particularly challenging today because there are more devices than people, and attackers are becoming more innovative. Successful cybersecurity approach has multiple layers of protection spread across the computers, networks, programs, or data that one intends to keep safe."
  },
  {
    name: "Cloud Computing Models",
    text: "Cloud computing is the on-demand availability of computer system resources, especially data storage and computing power, without direct active management by the user. Large clouds often have functions distributed over multiple locations, each location being a data center. Cloud computing relies on sharing of resources to achieve coherence and economies of scale. The three main models are Infrastructure as a Service (IaaS), Platform as a Service (PaaS), and Software as a Service (SaaS). Cloud providers use virtualization technologies to offer scalable resources over the internet."
  },
  {
    name: "Software Engineering Methodologies",
    text: "Software engineering is the systematic application of engineering approaches to the development of software. Methodologies like Agile and DevOps have transformed how teams build and ship code. Agile focuses on iterative development, continuous feedback, and rapid releases. DevOps merges development and operations to automate the deployment pipeline, ensuring that software can be released reliably and at any time. Continuous Integration and Continuous Deployment (CI/CD) are critical practices in modern software engineering."
  },
  {
    name: "Artificial Intelligence vs Human Intelligence",
    text: "Artificial intelligence (AI) refers to the simulation of human intelligence in machines that are programmed to think like humans and mimic their actions. The term may also be applied to any machine that exhibits traits associated with a human mind such as learning and problem-solving. The ideal characteristic of artificial intelligence is its ability to rationalize and take actions that have the best chance of achieving a specific goal. While AI is highly capable of processing large volumes of data quickly, human intelligence involves emotional intelligence, creativity, and moral judgment."
  },
  {
    name: "Object-Oriented Programming Concepts",
    text: "Object-oriented programming (OOP) is a programming paradigm based on the concept of 'objects', which can contain data and code. The data is in the form of fields, often known as attributes or properties, and the code is in the form of procedures, often known as methods. The four fundamental concepts of OOP are encapsulation, abstraction, inheritance, and polymorphism. Encapsulation hides the internal state of an object, inheritance allows classes to inherit features from other classes, and polymorphism allows treating different objects uniformly."
  },
  {
    name: "Distributed Systems Architecture",
    text: "A distributed system is a computing environment in which various components are spread across multiple computers or other computing devices on a network. These devices split up the work, coordinating their efforts to complete the job more efficiently than if a single device had been responsible for the task. Key challenges in distributed systems include dealing with network latency, partial failures, concurrency control, and data consistency. Protocols like Paxos and Raft are often used to achieve consensus among distributed nodes."
  },
  {
    name: "Blockchain Technology Overview",
    text: "A blockchain is a decentralized, distributed, and oftentimes public, digital ledger consisting of records called blocks that are used to record transactions across many computers so that any involved block cannot be altered retroactively, without the alteration of all subsequent blocks. This allows the participants to verify and audit transactions independently and relatively inexpensively. A blockchain database is managed autonomously using a peer-to-peer network and a distributed timestamping server. They are authenticated by mass collaboration powered by collective self-interests."
  },
  {
    name: "Web Development Frameworks",
    text: "Web development frameworks are designed to support the development of web applications including web services, web resources, and web APIs. Frameworks provide a standard way to build and deploy web applications on the World Wide Web. They aim to alleviate the overhead associated with common activities performed in web development. For example, many web frameworks provide libraries for database access, templating frameworks, and session management. Popular frontend frameworks include React, Angular, and Vue, while backend frameworks include Express, Django, and Spring Boot."
  },
  {
    name: "Data Mining and Big Data",
    text: "Data mining is the practice of analyzing large databases in order to generate new information. It is the computational process of discovering patterns in large data sets involving methods at the intersection of artificial intelligence, machine learning, statistics, and database systems. The overall goal of the data mining process is to extract information from a data set and transform it into an understandable structure for further use. Big Data refers to data sets that are too large or complex to be dealt with by traditional data-processing application software."
  },
  {
    name: "Cryptographic Hash Functions",
    text: "A cryptographic hash function is a mathematical algorithm that maps data of arbitrary size to a bit array of a fixed size. It is a one-way function, meaning it is computationally infeasible to invert or reverse the computation. Ideal cryptographic hash functions have several properties: they are deterministic, quick to compute, infeasible to generate a message from its hash value, infeasible to find two different messages with the same hash value, and a small change to a message should change the hash value so extensively that the new hash value appears uncorrelated with the old hash value."
  },
  {
    name: "The Theory of Computation",
    text: "The theory of computation is the branch that deals with how efficiently problems can be solved on a model of computation, using an algorithm. The field is divided into three major branches: automata theory and formal languages, computability theory, and computational complexity theory. Automata theory studies abstract machines and problems they are able to solve. Computability theory determines whether a problem is solvable at all on a computer. Complexity theory classifies computational problems according to their inherent difficulty, primarily regarding time and space resources."
  },
  {
    name: "Quantum Computing Principles",
    text: "Quantum computing is a rapidly-emerging technology that harnesses the laws of quantum mechanics to solve problems too complex for classical computers. Classical computers encode information in bits, which take the value of 1 or 0. Quantum computers use quantum bits, or qubits, which can exist in multiple states simultaneously due to a phenomenon called superposition. Another key principle is entanglement, which allows qubits that are entangled to interact with each other instantly regardless of distance. This gives quantum computers the potential to perform certain calculations exponentially faster."
  },
  {
    name: "Internet of Things (IoT)",
    text: "The Internet of Things describes physical objects with sensors, processing ability, software, and other technologies that connect and exchange data with other devices and systems over the Internet or other communications networks. The field has evolved due to the convergence of multiple technologies, including ubiquitous computing, commodity sensors, increasingly powerful embedded systems, and machine learning. IoT allows objects to be sensed and controlled remotely across existing network infrastructure, creating opportunities for more direct integration of the physical world into computer-based systems."
  },
  {
    name: "Mobile Application Development",
    text: "Mobile application development is the process of making software for smartphones, tablets and digital assistants, most commonly for the Android and iOS operating systems. The software can be preinstalled on the device, downloaded from a mobile app store or accessed through a mobile web browser. Development often requires the use of specialized integrated development environments (IDEs) and Software Development Kits (SDKs). Cross-platform development tools like Flutter and React Native allow developers to write code once and deploy it on both major operating systems."
  }
];
// Ensure it is globally available in the browser
if (typeof window !== 'undefined') {
  window.RESUME_CORPUS = RESUME_CORPUS;
}
